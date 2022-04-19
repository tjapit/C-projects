/**
 *	@file edit.c
 *	@author Timothy Japit
 *	This component contains functions to edit the contents of the text
 *  representation, specifically to remove one or more lines and one or more
 *  columns.
 */
#include "edit.h"
#include "text.h"
#include <stdlib.h>
#include <string.h>

/**
 * Modifies the global text representation to remove lines in the given range.
 *
 * @param start start of the removal index
 * @param end end of the removal index
 */
void removeLines(int start, int end) {
    // Decrement indices to revert to 0-indexing
    const int startInd = start - 1, endInd = end - 1;

    // Index through the text representation
    int index;

    // String removal
    // Loop through from start to end
    for (index = startInd; index <= endInd; ++index) {
        // String length
        int len = strlen(textRep[index]);

        // Remove the chars from the string
        for (int i = 0; i < len; ++i) {
            textRep[index][i] = NUL_CHAR;
        }
    }

    // String movement
    // Copy destination index
    int cpyDest = startInd;

    // Loop from the end of removal until there are no more strings
    while (strlen(textRep[index]) > 0) {
        // Copy each line of string to occupy start -> end
        strcpy(textRep[cpyDest], textRep[index]);

        // Remove the chars from the string at the previous index
        int len = strlen(textRep[index]);
        for (int i = 0; i < len; ++i) {
            textRep[index][i] = NUL_CHAR;
        }

        // increment indices
        cpyDest++, index++;
    }
}

/**
 * Remove characters in the start .. end range on just one line (the one at index
 * given by the first parameter).
 *
 * @param lno row index for removal
 * @param start start of the removal index
 * @param end end of the removal index
 */
void editLine(int lno, int start, int end) {
    // Decrement indices to revert to 0-indexing
    const int startInd = start - 1, endInd = end - 1;

    // Check if string length is shorter than the start of removal index
    int len = strlen(textRep[lno]);
    if (len < start) {
        // do nothing
        return;
    } else {
        // character index in a line
        int charInd;
        // Loop through chars in a line from start to end
        for (charInd = startInd; charInd <= endInd; ++charInd) {
            // break out once all characters have been removed
            if (textRep[lno][charInd] == NUL_CHAR)
                break;
            // Remove the chars at the given column
            textRep[lno][charInd] = NUL_CHAR;
        }

        // Copy destination index
        int cpyDest = startInd;
        // while there are characters left in the line
        while (textRep[lno][charInd] != NUL_CHAR) {
            // Shift the characters to the left
            textRep[lno][cpyDest] = textRep[lno][charInd];

            // Replace the characters shifted with NUL char
            textRep[lno][charInd] = NUL_CHAR;

            // move to next available char and increment copy destination index
            cpyDest++, charInd++;
        }
    }

}

/**
 * Uses editLine() to remove the given range of columns from all the lines of
 * the text.
 *
 * @param start start of the removal index
 * @param end end of the removal index
 */
void removeCols(int start, int end) {
    // Loop until no more strings
    int index = 0;
    while (strlen(textRep[index]) > 0) {
        // Edit the current line
        editLine(index, start, end);
        // increment index
        index++;
    }
}
