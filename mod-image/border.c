/**
 * @file border.c
 * @author Timothy Japit
 * This program adds a 4-pixel border around the entire image. It contains
 * the main() function for border, along with other utility functions.
 */
#include <stdlib.h>
#include <stdio.h>
#include "image.h"

/** The size of the border to put around the image. */
#define PADDING 4

void borderImage(int height, int width, int borderedHeight, int borderedWidth,
        unsigned char[height][width][DEPTH],
        unsigned char[borderedHeight][borderedWidth][DEPTH]);
void newPixArray(int borderedHeight, int borderedWidth,
        unsigned char[borderedHeight][borderedWidth][DEPTH]);

/**
 * Program starting point.
 *
 * @return exit status code
 */
int main() {

    // Reads image format header
    checkType();

    // Reads the width of the image from the header
    int width = readDimension();
    // Reads the height of the image from the header
    int height = readDimension();
    // height with border
    int borderedHeight = height + 2 * PADDING;
    // width with border
    int borderedWidth = width + 2 * PADDING;

    // Reads maximum intensity header
    checkRange();

    // Reads pixel data from input image
    unsigned char pix[height][width][DEPTH];
    readPixels(height, width, pix);

    // create new array with zeros with the new size
    unsigned char newPix[borderedHeight][borderedWidth][DEPTH];
    newPixArray(borderedHeight, borderedWidth, newPix);

    // Process the image array to add border
    borderImage(height, width, borderedHeight, borderedWidth, pix, newPix);

    // Writes pixel data to output image
    writeImage(borderedHeight, borderedWidth, newPix);
}

/**
 * Adds the image RGB values to the center of the array with all zero RGB
 * values.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param borderedHeight height with added padding
 * @param borderedWidth width with added padding
 * @param pix[][][] array of the input image's RGB values
 * @param newPix[][][] array of empty RGB values with the size of the image
 *                      and twice the added padding
 */
void borderImage(int height, int width, int borderedHeight, int borderedWidth,
        unsigned char pix[height][width][DEPTH],
        unsigned char newPix[borderedHeight][borderedWidth][DEPTH]) {
    // row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // add the RGB value to the corresponding kernel
                newPix[i + PADDING][j + PADDING][k] = pix[i][j][k];
            }
        }
    }
}

/**
 * Creates a new array that has the new size with the added padding.
 *
 * @param borderedHeight height with added padding
 * @param borderedWidth width with added padding
 * @param newPix[][][] array of empty RGB values with the size of the image
 *                      and twice the added padding
 */
void newPixArray(int borderedHeight, int borderedWidth,
        unsigned char newPix[borderedHeight][borderedWidth][DEPTH]) {
    // row/height
    for (int i = 0; i < borderedHeight; ++i) {
        // column/width/each pixel
        for (int j = 0; j < borderedWidth; ++j) {
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // define zero as the value for each kernel
                newPix[i][j][k] = 0;
            }
        }
    }
}
