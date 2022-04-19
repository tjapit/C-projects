/**
 * @file brighten.c
 * @author Timothy Japit
 * This program brightens the input image. It contains the main() function for
 * brighten, along with other utility functions.
 */
#include <stdlib.h>
#include <stdio.h>
#include "image.h"

/** How much to add to the intensity values of a pixel. */
#define DELTA 32

void brightenImage(int height, int width, unsigned char[height][width][DEPTH]);

/**
 * Program starting point.
 *
 * @return exit status code
 */
int main() {
    int width, height;
    // Reads image format from the header
    checkType();

    // Reads the width of the image from the header
    width = readDimension();
    // Reads the height of the image from the header
    height = readDimension();

    // Reads maximum intensity from the header
    checkRange();

    // Reads pixel data from input image
    unsigned char pix[height][width][DEPTH];
    readPixels(height, width, pix);

    // Process the image array to add brightness
    brightenImage(height, width, pix);

    // Writes pixel data to output image
    writeImage(height, width, pix);
}

/**
 * Brightens the image by adding a pre-determined value to all the RGB values.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param pix[][][] array of the image's RGB values
 */
void brightenImage(int height, int width,
        unsigned char pix[height][width][DEPTH]) {
    // row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // if the rgb value added with delta is less than max intensity,
                if (pix[i][j][k] + DELTA < STD_RANGE) {
                    // add delta to rgb value
                    pix[i][j][k] += DELTA;
                } else {
                    // else assign rgb value to max intensity
                    pix[i][j][k] = STD_RANGE;
                }
            }
        }
    }
}
