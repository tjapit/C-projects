/**
 *	@file text.c
 *	@author Timothy Japit
 *	This component defines the array of strings used to represent lines of text
 *  from the input file. It also has functions to read this representation from
 *  a file and write the resulting text out to a file.
 */
#include "text.h"
#include "edit.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Prints error message if line exceeds max character limit and exits
 * unsuccessfully.
 */
static void charLimit() {
    fprintf(stderr, "Line too long\n");
    exit(EXIT_FAILURE);
}

/**
 * Prints error message if file exceeds max line limit and exits
 * unsuccessfully.
 */
static void lineLimit() {
    fprintf(stderr, "Too many lines\n");
    exit(EXIT_FAILURE);
}

/**
 * Count number of lines in the text representation.
 *
 * @return number of lines in the text representation
 */
static int lineCounter() {
    int lines = 0;
    for (int i = 0; i < MAX_LINES; ++i) {
        if (strlen(textRep[i]))
            lines++;
        else
            break;
    }
    return lines;
}

/**
 * Reads text from the given file and stores it in the global text
 * representation. If the program reading from standard input, then the given
 * file pointer will be stdin.
 *
 * @param *fp pointer to the input file
 */
void readFile(FILE *fp) {
    // Allocate text representation
    textRep = (char (*)[MAX_CHARS + 1]) malloc(
    MAX_LINES * (MAX_CHARS + 1) * sizeof(char));

    // pre-allocate buffer memory
    char *buffer = (char*) malloc((MAX_CHARS + 2) * sizeof(char));

    // Text file
    // Read string from file to a buffer
    int lines = 0;
    while (fgets(buffer, MAX_CHARS + 2, fp) != NULL) {
        // Error checking for max chars
        if (strlen(buffer) > MAX_CHARS)
            charLimit();

        // Increment line counter
        lines++;

        // Error checking for max lines
        if (lines >= MAX_LINES)
            lineLimit();

        // Place string in representation
        strcpy(textRep[lines - 1], buffer);
    }

    // free buffer memory
    free(buffer);
}

/**
 * Writes out the text from the global text representation to the given output
 * file. If the program is writing to standard output, then the given file
 * pointer should be stdout.
 *
 * @param *fp pointer to the output file
 */
void writeFile(FILE *fp) {
// number of lines in text representation
    int lines = lineCounter();
// Loop through text representation
    for (int i = 0; i < lines; ++i) {
        // check for duplicate newline tags
        int tagCnt = 0;
        for (int j = 0; j < strlen(textRep[i]); ++j) {
            // Counting occurence of newlines
            if (textRep[i][j] == '\n')
                tagCnt++;
        }

        // If newline already exists
        if (tagCnt)
            // Print out text representation to file without new line
            fprintf(fp, "%s", textRep[i]);
        else
            // Print out text representation to file with new line
            fprintf(fp, "%s\n", textRep[i]);
    }
}
