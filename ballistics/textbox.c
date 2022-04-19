/**
 * @file textbox.c
 * @author Timothy Japit
 * This program prints out a text box around a given text input, wraps the text
 * inside a border of stars with each line containing 60 characters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Width of each line of text in the box. */
#define LINE_WIDTH 60

/** Symbol used to draw the border around the box. */
#define BORDER '*'

/** Extra characters for the text box. */
#define EXTRA_CHAR 2

/** Function prototypes */
void repeatChars(char, int);
bool paddedLine();

/**
 * Program starting point, receives the text as a standard input and prints it
 * out with a border around it.
 *
 * @return program exit code
 */
int main() {
    // print top border
    repeatChars(BORDER, LINE_WIDTH + EXTRA_CHAR);

    // while lines exist, print lines
    while (paddedLine())
        ;

    // print bottom border
    repeatChars(BORDER, LINE_WIDTH + EXTRA_CHAR);

    // exit
    return EXIT_SUCCESS;
}

/**
 * Prints out a line of the given character to the number of repetition
 * followed by a new line.
 *
 * @param ch character to be repeatedly printed
 * @param count number of repetition
 */
void repeatChars(char ch, int count) {
    // number of repetition
    for (int i = 0; i < count; i++) {
        // print the characters
        printf("%c", ch);
    }
    // new line
    printf("\n");
}

/**
 * Reads and prints a single line of text inside the border, returns false if
 * there is no more text to put in the box. If the input line of text isn't
 * long enough, it will add extra spaces at the end to make the box rectangular.
 *
 * @return false if there is no more text to put in the box
 */
bool paddedLine() {
    // reads from standard input
    int c = getchar();
    // if character read is not EOF,
    if (c != EOF) {
        // keeps track of spaces left
        int spacesLeft = LINE_WIDTH;
        // prints left border
        printf("%c", BORDER);
        // while the char reader hasn't encountered line break
        while (c != '\n') {
            // if there are spaces left and char is not a line break
            if (spacesLeft) {
                // place character
                (void) putchar(c);
                // read character
                c = getchar();
                // subtract spaces left
                spacesLeft--;
            } else {
                c = getchar();
            }
        }

        // while there are space left
        while (spacesLeft) {
            // prints spaces to keep rectangular shape
            printf(" ");
            spacesLeft--;

        }
        // prints right border and new line
        printf("%c\n", BORDER);

        // more characters, no space left, eat more characters

        // return true to read next line
        return true;
    }

    // return false if last character read was EOF
    return false;
}

