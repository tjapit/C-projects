/**
 *	@file chop.c
 *	@author Timothy Japit
 *	This is the main component of the program. It will parse the command line
 *	arguments, open the input and output files and use the other components to
 *	read the input, remove selected lines and columns, then right out the
 *	resulting text.
 */
#include "text.h"
#include "edit.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void readFile(FILE*);
void writeFile(FILE*);
void removeLines(int, int);
void removeCols(int, int);

/**
 * Prints error message for invalid arguments to standard error and exits
 * unsuccessfully.
 */
static void invalidArgs() {
    fprintf(stderr,
            "invalid arguments\nusage: chop command* (infile|-) (outfile|-)\n");
    exit(EXIT_FAILURE);
}

/**
 * Prints error message if file can't open to standard error and exits
 * unsuccessfully.
 */
static void fileBad(char *filename) {
    fprintf(stderr, "Can't open file: %s\n", filename);
    exit(EXIT_FAILURE);
}

/**
 * Program starting point.
 *
 * @param argc number of arguments
 * @param *argv[] pointer to the array of arguments
 * @return exit status code
 */
int main(int argc, char *argv[]) {
    // Checks standard input/file
    char *input = argv[argc - 2];
    FILE *ip;
    if (strcmp(input, "-") == 0) {
        // Read from standard input
        ip = stdin;
    } else {
        // Opens pointer to the input file
        ip = fopen(input, "r");
        if (!ip) {
            // Check bad input file
            fileBad(input);
        }
    }

    // Read file
    readFile(ip);

    // Edit the lines while there are commands
    int currentArg = 1;
    while (currentArg < argc - 2) {
        // Record command
        char *command;
        int start, end;
        command = argv[currentArg];

        // Removing a single line check if next command is a digit
        if (strcmp(command, "line") == 0 && isdigit(argv[currentArg + 1][0])) {
            // Get index
            sscanf(argv[currentArg + 1], "%d", &start);
            end = start;
            // Exit program if index is equal to or less than 0
            if (start <= 0) {
                invalidArgs();
            }

            // Remove lines from start to end
            removeLines(start, end);
            // Move current argument index
            currentArg++;

        }
        // Removing multiple lines
        else if (strcmp(command, "lines")
                == 0&& isdigit(argv[currentArg + 1][0]) &&
                isdigit(argv[currentArg + 2][0])) {
            // Get index
            sscanf(argv[currentArg + 1], "%d", &start);
            sscanf(argv[currentArg + 2], "%d", &end);
            // Exit program if index is equal to or less than 0 or end is less than start
            if (start <= 0 || end < start) {
                invalidArgs();
            }

            // Remove lines from start to end
            removeLines(start, end);
            // Move current argument index
            currentArg += 2;
        }
        // Removing a single column
        else if (strcmp(command, "col") == 0 && isdigit(argv[currentArg + 1][0])) {
            // Get index
            sscanf(argv[currentArg + 1], "%d", &start);
            end = start;
            // Exit program if index is equal to or less than 0
            if (start <= 0) {
                invalidArgs();
            }

            // Remove lines from start to end
            removeCols(start, end);
            // Move current argument index
            currentArg++;
        }
        // Removing multiple columns
        else if (strcmp(command, "cols")
                == 0&& isdigit(argv[currentArg + 1][0]) && isdigit(argv[currentArg + 2][0])) {
            // Get index
            sscanf(argv[currentArg + 1], "%d", &start);
            sscanf(argv[currentArg + 2], "%d", &end);
            // Exit program if index is equal to or less than 0 or end is less than start
            if (start <= 0 || end < start) {
                invalidArgs();
            }

            // Remove lines from start to end
            removeCols(start, end);
            // Move current argument index
            currentArg += 2;
        }
        // Invalid command
        else {
            invalidArgs();
        }

        // Move current argument index
        currentArg++;
    }

    // Checks standard output/file
    char *output = argv[argc - 1];
    FILE *op;
    if (strcmp(output, "-") == 0) {
        // Write to standard output
        op = stdout;
    } else {
        // Opens pointer to the output file
        op = fopen(output, "w");
        if (!op) {
            // Check bad output file
            fileBad(output);
        }
    }

    // Write to file
    writeFile(op);

    // Close files and free any memory dynamically allocated
    free(textRep);
    fclose(ip);
    fclose(op);
}
