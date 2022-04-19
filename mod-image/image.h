/**
 * @file image.h
 * @author Timothy Japit
 * This file contains the global variable and function prototype of image.c.
 */

/** Maximum intensity value expected for images. */
#define STD_RANGE 255

/** Number of color components per pixel */
#define DEPTH 3

/** Exit code for a bad magic number at the top. */
#define BAD_HEADER 100

/** Exit code for bad or missing pixel data */
#define BAD_PIXEL 101

/** This function is used to read the magic number at the start of an input image. */
void checkType();

/** This function is used to read the width and height. */
int readDimension();

/** This function reads the max intensity value at the end of the image header. */
void checkRange();

/** This function reads all the pixel data for an image. */
void readPixels(int height, int width, unsigned char[height][width][DEPTH]);

/** This function writes the output image to standard output. */
void writeImage(int height, int width, unsigned char[height][width][DEPTH]);
