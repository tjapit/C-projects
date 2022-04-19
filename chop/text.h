/**
 *	@file text.h
 *	@author Timothy Japit
 *	This component defines the array of strings used to represent lines of text
 *	from the input file. It also has functions to read this representation from
 *	a file and write the resulting text out to a file.
 */
#include <stdio.h>

/** Maximum number of characters in the input line. */
#define MAX_CHARS 100

/** Maximum number of lines in the file. */
#define MAX_LINES 10000

/** Text representation */
char (*textRep)[MAX_CHARS + 1];

/**
 * Reads text from the given file and stores it in the global text
 * representation. If the program reading from standard input, then the given
 * file pointer will be stdin.
 */
void readFile(FILE *);

/**
 * Writes out the text from the global text representation to the given output
 * file. If the program is writing to standard output, then the given file
 * pointer should be stdout.
 */
void writeFile(FILE *);
