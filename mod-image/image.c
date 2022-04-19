/**
 * @file image.c
 * @author Timothy Japit
 * This component provides functions for reading and writing PPM images and
 * makes sure they are in the right format.
 */
#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Format header length. */
#define FORMAT 3
/** Readable image format. */
const char readableType[] = "P3";

/**
 * This function reads the magic number at the start of the input image.
 */
void checkType() {
    char type[FORMAT] = "";
    // reads image type header
    scanf("%s", type);
    // exits the program if the image type is not PPM
    if (strcmp(type, readableType) != 0) {
        exit(BAD_HEADER);
    }
}

/**
 * This function reads a dimension of size of the input image.
 */
int readDimension() {
    int dim;
    // reads image dimension
    scanf("%d", &dim);
    // exits the program if the dimension is invalid
    if (dim < 1) {
        exit(BAD_HEADER);
    }
    return dim;
}

/**
 * This function reads the max intensity value of the RGB values.
 */
void checkRange() {
    unsigned char intensity;
    // reads max intensity value
    scanf("%hhu", &intensity);
    // exits the program if the max intensity is invalid
    if (intensity != STD_RANGE) {
        exit(BAD_HEADER);
    }
}

/**
 * This function reads all the pixel data for an image.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param pix[][][] array of the image's RGB values
 */
void readPixels(int height, int width, unsigned char pix[height][width][DEPTH]) {
    unsigned char rgb;
    // row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // get rgb values
                scanf("%hhu", &rgb);
                // error check value
                if (rgb < 0 || rgb > STD_RANGE) {
                    exit(BAD_PIXEL);
                }
                // assign rgb value to pixel
                pix[i][j][k] = rgb;
            }
        }
    }
}

/**
 * This function writes the output image to standard output.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param pix[][][] array of the image's RGB values
 */
void writeImage(int height, int width, unsigned char pix[height][width][DEPTH]) {
    // PPM format header
    (void) printf("%s\n", readableType);
    // size header
    (void) printf("%d %d\n", width, height);
    // max intensity header
    (void) printf("%d\n", STD_RANGE);

    // writing image to output
    // row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // write without space proceeding space at the end of line
                if (j == width - 1 && k == DEPTH - 1) {
                    (void) printf("%hhu", pix[i][j][k]);
                } else {
                    // write each rgb value to its ascribed pixel
                    (void) printf("%hhu ", pix[i][j][k]);
                }
            }
        }
        // move to next row
        (void) printf("\n");
    }
}
