/**
 *	@file reading.c
 *	@author Timothy Japit
 *	Contains main(), reads text files containing catalogs of books in specific
 *	format, uses functions from input.c and catalog.c to store the catalog, and
 *	allows the user to add/remove books to a reading list.
 */
#include "input.h"
#include "catalog.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/** Maximum characters for a command. */
#define MAX_COMMAND 7

/**
 * Exits the program when the user uses incorrect syntax with status of 1.
 */
static void usage() {
    fprintf(stderr, "usage: reading <book-list>*\n");
    exit(EXIT_FAILURE);
}

/**
 * This function reads a single line of input from the given file and returns it
 * as a string inside a block of dynamically allocated memory.
 *
 * @param fp pointer to the input file
 * @return a dynamically allocated memory containing lines from the file
 */
char* readLine(FILE *fp);

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

/**
 * This function lists all books within the reading list added by the user,
 * sorted by their reading level, and prints out the average level and sum
 * total of the number of words of all the books in the list.
 *
 * @param rlist pointer to a Catalog struct that stores the reading list
 */
void listReading(Catalog *rlist);

/**
 * This function adds a book with the given ID from the catalog to the reading
 * list. If the book ID does not exist in the catalog or if it is already in
 * the reading list, the user will be notified.
 *
 * @param cat pointer to the Catalog
 * @param rlist pointer to the reading list Catalog struct
 * @param id ID of the book to be copied
 */
void addReading(Catalog *cat, Catalog *rlist, int id);

/**
 * This function removes a book with the given ID from the catalog to the
 * reading list. If the book ID does not exist in the catalog, the user will be
 * notified.
 *
 * @param rlist pointer to the reading list Catalog struct
 * @param id ID of the book to be copied
 */
void removeReading(Catalog *rlist, int id);

/**
 * This function frees the memory used to store the reading list, including
 * freeing space for all the books, freeing the resizable array of book
 * pointers and freeing space for the Catalog struct itself.
 *
 * @param cat pointer to the Catalog struct
 */
void freeList(Catalog *cat);

/**
 * Program starting point.
 *
 * @param argc number of commands from the command line
 * @param argv array of commands from the command line
 * @return exit status code
 */
int main(int argc, char *argv[]) {
    // Check usage of reading command
    if (argc < 2) {
        usage();
    }

    // Catalog pointer
    Catalog *cat = makeCatalog();

    // Reading list pointer
    Catalog *rlist = makeCatalog();

    // Reads file(s) and delegates to readCatalog()
    for (int i = 1; i < argc; ++i) {
        // Read from file to Catalog
        readCatalog(cat, argv[i]);
    }

    // Prompts user and reads commands until "quit" is entered
    char *line;
    double min, max;
    while (1) {
        // Prompt and read line
        printf("cmd> ");
        line = readLine(stdin);

        // EOF termination
        if (!line) {
            free(line);
            break;
        }

        // Echo command
        printf("%s", line);


        // Parsing command, re-initializing variables
        char command[MAX_COMMAND] = "";
        min = 0;
        max = 0;
        sscanf(line, "%s %lf %lf", command, &min, &max);

        // Possible commands
        // catalog
        if (strcmp(command, "catalog") == 0) {
            // List all books
            listAll(cat);
        }
        // level
        else if (strcmp(command, "level") == 0) {
            // Error check for level
            if (min > max) {
                // minimum level range is greater than maximum
                printf("Invalid command\n");
            }
            else if (min == 0 || max == 0) {
                printf("Invalid command\n");
            }
            else {
                // List books within level range
                listLevel(cat, min, max);
            }
        }
        // add
        else if (strcmp(command, "add") == 0) {
            // Error checking for book ID
            if (max != 0 || min <= 0) {
                // extra argument or invalid id
                printf("Invalid command\n");
            } else {
                // casting id to integer
                int id;
                id = (int) min;

                // add book to reading list
                addReading(cat, rlist, id);
            }
        }
        // remove
        else if (strcmp(command, "remove") == 0) {
            // Error checking for book ID
            if (max != 0 || min <= 0) {
                // extra argument or invalid id
                printf("Invalid command\n");
            } else {
                // casting id to integer
                int id;
                id = (int) min;

                // remove book from reading list
                removeReading(rlist, id);
            }
        }
        // list
        else if (strcmp(command, "list") == 0) {
            listReading(rlist);
        }
        // quit
        else if (strcmp(command, "quit") == 0) {
            free(line);
            break;
        }
        // invalid command
        else {
            printf("Invalid command\n");
        }

        // Print extra line to space between each user commands
        printf("\n");

        // Free line allocated memory from readLine() and command
        free(line);
    }

    // Free up Catalog and reading list memory
    freeCatalog(cat);
    freeList(rlist);

    return EXIT_SUCCESS;
}

