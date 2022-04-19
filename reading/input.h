/**
 *	@file input.h
 *	@author Timothy Japit
 *	This component contains a function that reads a single line from a file and
 *	returns a string inside a block of dynamically allocated memory.
 */
#include <stdio.h>

/** Maximum number of characters in a line that is readable. */
#define LINE_LENGTH 1000

/**
 * This function reads a single line of input from the given file and returns it
 * as a string inside a block of dynamically allocated memory.
 *
 * @param fp pointer to the input file
 * @return a dynamically allocated memory containing lines from the file
 */
char* readLine(FILE *fp);
