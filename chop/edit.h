/**
 *	@file edit.h
 *	@author Timothy Japit
 *	This component contains functions to edit the contents of the text
 *	representation, specifically to remove one or more lines and one or more
 *	columns.
 */

/** Nul char representation. */
#define NUL_CHAR '\0'

/**
 * Modifies the global text representation to remove lines in the given range.
 */
void removeLines(int, int);

/**
 * Remove characters in the start .. end range on just one line (the one at index
 * given by the first parameter).
 */
void editLine(int, int, int);

/**
 * Uses editLine() to remove the given range of columns from all the lines of
 * the text.
 */
void removeCols(int, int);
