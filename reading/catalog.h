/**
 *	@file catalog.h
 *	@author Timothy Japit
 *	This component contains function to make a Catalog, read books from file,
 *	list the catalog, and free up memory of the catalog.
 */
#include <stdio.h>
#include <stdbool.h>

/** Initial capacity of books. */
#define INITIAL_CAP 5

/** Maximum number of characters for book titles. */
#define MAX_TITLE 38

/** Maximum number of characters for book author. */
#define MAX_AUTHOR 20

/** Resize multiplier. */
#define RESIZE_MULT 2

/**
 * Struct for each Book with a field for the ID, title, author, reading level,
 * and the word count for the book.
 *
 * @param id ID of the Book
 * @param title title of the Book up to 38 characters
 * @param author name of the author of the Book up to 20 characters
 * @param level reading level of the Book
 * @param words word count of the Book
 */
typedef struct BookStruct {
    int id;
    char *title;
    char *author;
    double level;
    int words;
} Book;

/**
 * Struct for the Catalog of books from a list containing the list of books,
 * the number of books in the Catalog, and the storing capacity of the Catalog.
 *
 * @param books list of books in the Catalog
 * @param count number of books in the Catalog
 * @param capacity storing capacity of the Catalog
 */
typedef struct CatalogStruct {
    Book *books;
    int count;
    int capacity;
} Catalog;

/**
 * This function dynamically allocates storage for the catalog, initializes its
 * fields (to store a resizable array), and returns a pointer to it.
 *
 * @return pointer to the Catalog
 */
Catalog* makeCatalog();

/**
 * This function frees the memory used to store the catalog, including freeing
 * space for all the books, freeing the resizable array of book pointers and
 * freeing space for the Catalog struct itself.
 *
 * @param cat pointer to the Catalog struct
 */
void freeCatalog(Catalog *cat);

/**
 * This function reads all the books from a book list file with the given name.
 * It makes an instance of the Book struct for each one and stores a pointer to
 * that book in the resizable array.
 *
 * @param cat pointer to the Catalog struct
 * @param filename pointer to the file
 */
void readCatalog(Catalog *cat, char const *filename);

/**
 * This function lists all the books in the catalog, sorted by ID number. The
 * reading component can call this in response to the user entering the catalog
 * command.
 *
 * @param cat pointer to the Catalog struct
 */
void listAll(Catalog *cat);

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
void listLevel(Catalog *cat, double min, double max);

