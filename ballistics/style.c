/**
 * @file style.c
 * @author Timothy Japit
 * This program produces a randomly-generated paragraph of text. Prints a
 * paragraph of a given number of lines of words and the total number of
 * words in the paragraph.
 */

#include <stdio.h>
#include <stdlib.h>

/** Length limit for a line of words. */
#define LENGTH_LIMIT 72

/** Number of letters in the alphabet */
#define ALPHABET 26

/** Start of the lower-case letters from the ASCII table */
#define LOWERCASE_ASCII 97

/**
 * Prints a given number of random lower-case letter.
 *
 * @param x number of letters to be printed
 */
void printWord(int x) {
    // prints letters up to the given length
    for (int i = 0; i < x; i++) {
        // Print a random lower-case letter.
        printf("%c", LOWERCASE_ASCII + rand() % ALPHABET);
    }
}

/**
 * Prints a line of the letters and returns the number of words in the line.
 *
 * @return number of words in the line
 */
int printLine() {
    // keeping track of the number of words, position, and spaces
    int count = 0, pos = 0, space = 0;
    // random length of each word
    int len = 1 + rand() % 10;
    // Print a line of words up to a limited length.
    while (pos + len + space < LENGTH_LIMIT) {
        // if there is space left,
        if (space > 0) {
            // print space
            printf(" ");
        }
        // print the random letters to the random length
        printWord(len);
        // position of the cursor for next print
        pos += len + space;
        // random length of the next word
        len = 1 + rand() % 10;
        space = 1;
        // number of words
        count += 1;
    }

    // newline
    printf("\n");

    // number of words in a line
    return count;
}

/**
 * Prints a paragraph with the given number of lines.
 *
 * @param n number of lines in the paragraph
 * @return number of words in the paragraph
 */
int printParagraph(int n) {
    // keeping track of the total number of words
    int total = 0;

    // print each line until the number of given lines
    for (int i = 0; i < n; i++)
        // prints line and adds the number of words in the line to the total
        total += printLine();

    // return total number of words
    return total;
}

/**
 * Program starting point, prints a paragraph with the given number of lines
 * with random lower-case letters and keeps track of the number of total
 * words.
 *
 * @return program exit status code
 */
int main() {
    // number of lines in the paragraph
    int lines = 10;

    // prints a paragraph with a given number of lines
    int w = printParagraph(lines);

    // prints the total number of words in the paragraph
    printf("Words: %d\n", w);

    // exits from the program
    return EXIT_SUCCESS;
}
