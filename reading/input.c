/**
 *	@file input.c
 *	@author Timothy Japit
 *	This component contains a function that reads a single line from a file and
 *  returns a string inside a block of dynamically allocated memory.
 */
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This function reads a single line of input from the given file and returns it
 * as a string inside a block of dynamically allocated memory.
 *
 * @param fp pointer to the input file
 * @return a dynamically allocated memory containing lines from the file
 */
char* readLine(FILE *fp) {
    // Dynamically allocate char pointer
    char * line = (char *) malloc(LINE_LENGTH * sizeof(char));

    // Read each line with fgets()
    if (fgets(line, LINE_LENGTH, fp) == NULL) {
        /**
         * Note: because fgets() returns null at eof, the address is lost within
         *          this function when called by another function
         *          (i.e. readCatalog()), the caller cannot free the memory, thus
         *          the memory must be freed within the callee.
         */
        free(line);
        return NULL;
    } else {
        // Otherwise return line
        return line;
    }
}

