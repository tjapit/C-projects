/**
 *	@file catalog.c
 *	@author Timothy Japit
 *	This component contains function to make a Catalog, read books from file,
 *  list the catalog, and free up memory of the catalog.
 */
#include "catalog.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Number of spaces for ID field. */
#define SPACES_ID 5

/**
 * Unable to read file error message and exit with status of 1.
 *
 * @param filename name of the file
 */
static void badfile(const char *filename) {
    fprintf(stderr, "Can't open file: %s\n", filename);
    exit(EXIT_FAILURE);
}

/**
 * Line from the input file is missing details.
 *
 * @param filename name of the file
 */
static void badline(const char *filename) {
    fprintf(stderr, "Invalid book list: %s\n", filename);
    exit(EXIT_FAILURE);
}

/**
 * Duplicate ID in the Catalog.
 *
 * @param id the duplicate ID
 */
static void duplicateID(int id) {
    fprintf(stderr, "Duplicate book id: %d\n", id);
    exit(EXIT_FAILURE);
}

/**
 * Prints the header for the list and catalog output.
 */
static void printHeader() {
    printf("%5s %38s %20s %5s %7s\n", "ID", "Title", "Author", "Level",
            "Words");
}

/**
 * Prints the book with the given metadata.
 *
 * @param id Book ID
 * @param title Book title
 * @param author Book author name
 * @param level Book reading level
 * @param words Book word count
 */
static void printBook(int id, char *title, char *author, double level,
        int words) {
    // Print id
    printf("%5d ", id);
    // For titles exceeding maximum
    if (strlen(title) > MAX_TITLE) {
        // Shorten book title before printing
        char *titleShorten = (char*) malloc(MAX_TITLE * sizeof(char));
        memcpy(titleShorten, title, MAX_TITLE - 2); // cut original title
        titleShorten[MAX_TITLE - 2] = '\0'; // terminate string
        printf("%36s.. ", titleShorten);
        free(titleShorten);
    } else {
        // Print normally
        printf("%38s ", title);
    }

    // For author names exceeding maximum
    if (strlen(author) > MAX_AUTHOR) {
        // Shorten author name before printing
        char *authorShorten = (char*) malloc(MAX_AUTHOR * sizeof(char));
        memcpy(authorShorten, author, MAX_AUTHOR - 2); // cut original author
        authorShorten[MAX_AUTHOR - 2] = '\0'; // terminate string
        printf("%18s.. ", authorShorten);
        free(authorShorten);
    } else {
        // Print normally
        printf("%20s ", author);
    }

    // Print level and word count
    printf("%5.1lf %7d\n", level, words);
}

/**
 * Comparison function for qsort() that looks at 2 Books' ID, returns a negative
 * integer if the first Book ID is less than the second Book ID and a positive
 * integer if the first Book level is greater than the second Book ID. If two
 * IDs are the same, it returns a zero.
 *
 * @param book1 first Book to be compared
 * @param book2 second Book to be compared
 * @return negative, positive integer, or zero
 */
static int idcmp(const void *book1, const void *book2) {
    // Comparing the IDs by first casting void pointers to Book pointers
    if (((Book*) book1)->id < ((Book*) book2)->id) {
        return -1;
    } else if (((Book*) book1)->id > ((Book*) book2)->id) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Comparison function that looks at 2 Books' reading level, returns a negative
 * integer if the first Book level is less than the second Book level and a
 * positive integer if the first Book level is greater than the second Book
 * level. If reading levels are the same, then books are ordered by ID.
 *
 * @param book1 first Book to be compared
 * @param book2 second Book to be compared
 * @return negative or positive integer
 */
static int lvlcmp(const void *book1, const void *book2) {
    // Comparing the levels by first casting void pointers to Book pointers
    if (((Book*) book1)->level == ((Book*) book2)->level) {
        return idcmp(book1, book2);
    } else if (((Book*) book1)->level < ((Book*) book2)->level) {
        return -1;
    } else {
        return 1;
    }
}

/**
 * Test function to list all books in the catalog.
 *
 * @param book pointer to a Book
 * @param data void pointer to an arbitrary data
 * @return always returns true
 */
static bool printAll(Book const *book, void const *data) {
    return true;
}

/**
 * Test function to list books that are within the given level range.
 *
 * @param book pointer to a Book
 * @param data void pointer to levels
 * @return true if the Book is within the reading level range
 */
static bool printLevel(Book const *book, void const *data) {
    // Save ranges for debugging
    double min = ((double*) data)[0], max = ((double*) data)[1];

    // Return true if book level is within range
    if (book->level >= min && book->level <= max) {
        return true;
    } else {
        return false;
    }
}

/**
 * This is a static function in the catalog component used by listAll() and
 * listLevel() functions to actually report the list of books in the right
 * format. In addition to a pointer to the catalog, this function also takes
 * a pointer to a function (test) and a pointer to an arbitrary block of data
 * (data) to let the caller tell the function which particular books it should
 * print out.
 *
 * @param cat pointer to the Catalog struct
 * @param test pointer to a function that returns a boolean
 * @param data pointer to an arbitrary block of data
 */
static void listCatalog(Catalog *cat,
bool test(Book const *book, void const *data), void const *data) {
    // Check for any matching books before printing header
    bool matches = false;
    for (int i = 0; i < cat->count; ++i) {
        if (test(&cat->books[i], data)) {
            matches = true;
            break;
        }
    }

    // Print message if there are no matching books
    if (!matches || cat->count == 0) {
        printf("No matching books\n");
    }
    // Otherwise print matching books
    else {
        // Prints header
        printHeader();

        // Sorting the Catalog before printing
        if (test == printAll) {
            qsort(cat->books, cat->count, sizeof(Book), idcmp);
        } else if (test == printLevel) {
            qsort(cat->books, cat->count, sizeof(Book), lvlcmp);
        }

        // Prints book if test condition returns true
        for (int i = 0; i < cat->count; ++i) {
            if (test(&cat->books[i], data)) {
                printBook(cat->books[i].id, cat->books[i].title,
                        cat->books[i].author, cat->books[i].level,
                        cat->books[i].words);
            }
        }
    }
}

/**
 * This function dynamically allocates storage for the catalog, initializes its
 * fields (to store a resizable array), and returns a pointer to it.
 *
 * @return pointer to the Catalog
 */
Catalog* makeCatalog() {
    Catalog *cat = (Catalog*) malloc(sizeof(Catalog));
    cat->books = (Book*) malloc(INITIAL_CAP * sizeof(Book));
    cat->capacity = INITIAL_CAP;
    cat->count = 0;
    return cat;
}

/**
 * This function frees the memory used to store the catalog, including freeing
 * space for all the books, freeing the resizable array of book pointers and
 * freeing space for the Catalog struct itself.
 *
 * @param cat pointer to the Catalog struct
 */
void freeCatalog(Catalog *cat) {
    // free books in catalog
    for (int i = 0; i < cat->count; ++i) {
        free(cat->books[i].title);
        free(cat->books[i].author);
    }
    // free catalog
    free(cat->books);
    free(cat);
}

/**
 * This function reads all the books from a book list file with the given name.
 * It makes an instance of the Book struct for each one and stores a pointer to
 * that book in the resizable array.
 *
 * @param cat pointer to the Catalog struct
 * @param filename pointer to the file
 */
void readCatalog(Catalog *cat, char const *filename) {
    // File pointer
    FILE *fp = fopen(filename, "r");

    // Throws error when usage incorrect or files can't open
    if (!fp) {
        badfile(filename);
    }

    // Variable to store each line and book data
    char *line;
    int id, words;
    double level;
    char *title, *author;

    // Read file line-by-line until end of file
    while ((line = readLine(fp)) != NULL) {
        // Initialize book data
        id = 0, level = 0, words = 0;
        title = (char*) malloc(LINE_LENGTH * sizeof(char));
        author = (char*) malloc(LINE_LENGTH * sizeof(char));

        // Scan line and store book data
        sscanf(line,
                "%d\t%[]0-9a-zA-Z'.,;:?!()-&/\"\[ ]\t%[0-9a-zA-z',.()&- ]\t%lf\t%d",
                &id, title, author, &level, &words);

        // Throw error if line is bad
        if (id
                == 0|| level == 0 || words == 0 || title == NULL || author == NULL) {
//            printf("%d\n%s\n%s\n", id, title, author); //debugging characters needed for sscanf
            badline(filename);
        }

        // Throw error if ID is duplicate
        for (int i = 0; i < cat->count; ++i) {
            if (cat->books[i].id == id) {
                duplicateID(id);
            }
        }

        // Check for Catalog capacity
        if (cat->count >= cat->capacity) {
            // Doubling the capacity
            cat->capacity *= RESIZE_MULT;
            // Resizing the books array
            cat->books = (Book*) realloc(cat->books,
                    cat->capacity * sizeof(Book));
        }

        // Initialize title and author
        cat->books[cat->count].title = (char*) malloc(
        LINE_LENGTH * sizeof(char));
        cat->books[cat->count].author = (char*) malloc(
        LINE_LENGTH * sizeof(char));

        // Input book data to catalog
        cat->books[cat->count].id = id;
        strcpy(cat->books[cat->count].title, title);
        strcpy(cat->books[cat->count].author, author);
        cat->books[cat->count].level = level;
        cat->books[cat->count].words = words;

        // Increment book count
        cat->count += 1;

        // Free allocated memories
        free(title);
        free(author);
        free(line);
    }

    // Free allocated memories and close files
    fclose(fp);
}

/**
 * This function lists all the books in the catalog, sorted by ID number. The
 * reading component can call this in response to the user entering the catalog
 * command.
 *
 * @param cat pointer to the Catalog struct
 */
void listAll(Catalog *cat) {
    listCatalog(cat, printAll, NULL);
}

/**
 * This function lists all the books with a reading level between the given min
 * and max values (inclusive). The reading component calls this when the user
 * enters the level command. Books are sorted by reading level or by ID if they
 * have the same reading level.
 *
 * @param cat pointer to the Catalog struct
 * @param min minimum inclusive reading level
 * @param max maximum inclusive reading level
 */
void listLevel(Catalog *cat, double min, double max) {
    double range[] = { min, max };
    listCatalog(cat, printLevel, range);
}

/**
 * This function lists all books within the reading list added by the user,
 * sorted by the order of addition, and prints out the average level and sum
 * total of the number of words of all the books in the list.
 *
 * @param rlist pointer to a Catalog struct that stores the reading list
 */
void listReading(Catalog *rlist) {
    // Check if list is empty
    if (rlist->count == 0) {
        printf("List is empty\n");
    } else {
        // Print the header
        printHeader();

        // Keep track of the total level and total word count
        double sumLevel = 0;
        int sumWords = 0;

        // Print the books in the list
        for (int i = 0; i < rlist->count; ++i) {
            printBook(rlist->books[i].id, rlist->books[i].title,
                    rlist->books[i].author, rlist->books[i].level,
                    rlist->books[i].words);
            sumLevel += rlist->books[i].level;
            sumWords += rlist->books[i].words;
        }

        // Print average level and total word count
        printf("%*c %*c %*c %5.1lf %7d\n", SPACES_ID, ' ', MAX_TITLE, ' ',
        MAX_AUTHOR, ' ', (sumLevel / rlist->count), sumWords);
    }

}

/**
 * This function adds a book with the given ID from the catalog to the reading
 * list. If the book ID does not exist in the catalog or if it is already in
 * the reading list, the user will be notified.
 *
 * @param cat pointer to the Catalog
 * @param rlist pointer to the reading list Catalog struct
 * @param id ID of the book to be copied
 */
void addReading(Catalog *cat, Catalog *rlist, int id) {
    // Sort Catalog before bsearching
    qsort(cat->books, cat->count, sizeof(Book), idcmp);
    // Use bsearch to find book with a matching id
    Book *matchingBookCat;
    matchingBookCat = (Book*) bsearch(&id, cat->books, cat->count, sizeof(Book),
            idcmp);

    // bsearch returns null if no matching books are found
    if (!matchingBookCat) {
        // Print message no matching id found
        printf("Book ID is not in the catalog\n");
    } else {
        // Check if reading list is not empty
        if (rlist->count != 0) {
            // If list is not empty, check id
            bool matchingBookList = false;
            for (int i = 0; i < rlist->count; ++i) {
                if (rlist->books[i].id == id) {
                    matchingBookList = true;
                    break;
                }
            }

            if (matchingBookList) {
                // if there are matching id in the reading list, notify user
                printf("Book %d is already on the reading list\n", id);
                return;
            }

            // If number of books on the list is up to the capacity
            if (rlist->count >= rlist->capacity) {
                // Double the capacity
                rlist->capacity *= RESIZE_MULT;
                // Reallocate the list of books pointer
                rlist->books = (Book*) realloc(rlist->books,
                        rlist->capacity * sizeof(Book));
            }
        }


        // Copying book from the catalog to reading list
        memcpy(&rlist->books[rlist->count], matchingBookCat, sizeof(Book));
        // Increment book count in reading list
        rlist->count += 1;
    }
}

/**
 * This function removes a book with the given ID from the catalog to the
 * reading list. If the book ID does not exist in the catalog, the user will be
 * notified.
 *
 * @param rlist pointer to the reading list Catalog struct
 * @param id ID of the book to be copied
 */
void removeReading(Catalog *rlist, int id) {
    // If list is empty, don't remove anything
    if (rlist->count == 0) {
        printf("List is empty\n");
    } else {
        // Loop through reading list to look for matching id
        bool matchingBookList = false;
        int removalIndex;
        for (removalIndex = 0; removalIndex < rlist->count; ++removalIndex) {
            if (rlist->books[removalIndex].id == id) {
                matchingBookList = true;
                break;
            }
        }

        // No matching books, notify user
        if (!matchingBookList) {
            printf("Book %d is not on the reading list\n", id);
            return;
        }

        // Matching book, shift all books upwards if needed
        for (int i = removalIndex; i < rlist->count; ++i) {
            memmove(&rlist->books[i], &rlist->books[i + 1], sizeof(Book));
        }

        // Decrement book count in reading list
        rlist->count -= 1;
    }
}

/**
 * This function frees the memory used to store the reading list, including
 * freeing space for all the books, freeing the resizable array of book
 * pointers and freeing space for the Catalog struct itself.
 *
 * @param cat pointer to the Catalog struct
 */
void freeList(Catalog *cat) {
    /**
     * No need to free each author and title, because rlist only copies book
     * pointers
     */
    // free reading list and the pointer to books
    free(cat->books);
    free(cat);
}

