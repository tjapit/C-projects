/**
 * @file blur.c
 * @author Timothy Japit
 * This program uses a 3x3 Gaussian blur to all the pixels of the input image.
 * It contains the main() function for blur, along with other utility
 * functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

/** Kernel size. */
#define KERNEL_SIZE 3

/** 3x3 Gaussian distribution kernel.*/
const int kernelGaussian[KERNEL_SIZE][KERNEL_SIZE] = { { 1, 2, 1 }, { 2, 4, 2 },
        { 1, 2, 1 } };

/** Top left corner pixel */
#define CORNER_TOP_LEFT 0
/** Top right corner pixel */
#define CORNER_TOP_RIGHT 1
/** Bottom left corner pixel */
#define CORNER_BOTTOM_LEFT 2
/** Bottom right corner pixel */
#define CORNER_BOTTOM_RIGHT 3
/** Top edge pixel */
#define EDGE_TOP 4
/** Bottom edge pixel */
#define EDGE_BOTTOM 5
/** Left edge pixel */
#define EDGE_LEFT 6
/** Right edge pixel */
#define EDGE_RIGHT 7
/** Middle pixel */
#define MIDDLE 8

void blurImage(int height, int width, unsigned char[height][width][DEPTH],
        unsigned char[height][height][DEPTH]);
void newPixArray(int height, int width, unsigned char[height][width][DEPTH]);
int detectLocation(int, int, int, int);
void prepConvArrays(int height, int width, int, int, int,
        unsigned char[height][width][DEPTH], int[KERNEL_SIZE][KERNEL_SIZE],
        int[KERNEL_SIZE][KERNEL_SIZE], int);
unsigned char convolution(int[KERNEL_SIZE][KERNEL_SIZE],
        int[KERNEL_SIZE][KERNEL_SIZE]);
void zeros(int height, int width, int[height][width]);
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

    // Reads maximum intensity header
    checkRange();

    // Reads pixel data from input image
    unsigned char pix[height][width][DEPTH];
    readPixels(height, width, pix);

    // Create new array as output
    unsigned char newPix[height][width][DEPTH];
    newPixArray(height, width, newPix);

    // Process the image array to add blur and store into the new array
    blurImage(height, width, pix, newPix);

    // Writes pixel data to output image
    writeImage(height, width, newPix);
}

/**
 * Applies a Gaussian blur with a 3x3 kernel to the image and outputs it in
 * a new array.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param pix[][][] array of the input image's RGB values
 * @param newPix[][][] array of empty RGB values that has the same size as pix
 */
void blurImage(int height, int width, unsigned char pix[height][width][DEPTH],
        unsigned char newPix[height][width][DEPTH]) {
    // row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // detect pixel location
            int location = detectLocation(height, width, i, j);
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // initialize arrays for convolution
                int intensityMatrix[KERNEL_SIZE][KERNEL_SIZE];
                int kernel[KERNEL_SIZE][KERNEL_SIZE];
                zeros(KERNEL_SIZE, KERNEL_SIZE, intensityMatrix);
                zeros(KERNEL_SIZE, KERNEL_SIZE, kernel);

                // prepare arrays for convolution
                prepConvArrays(height, width, i, j, k, pix, intensityMatrix,
                        kernel, location);

                // calculate and output the new rgb value
                newPix[i][j][k] = convolution(intensityMatrix, kernel);
            }
        }
    }
}

/**
 * Detects the location of the current pixel and determines whether it is an
 * edge, a corner, or middle pixel.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param currentRow current height location of the pixel
 * @param currentCol current width location of the pixel
 * @return edge, corner, or middle designation
 */
int detectLocation(int height, int width, int currentRow, int currentCol) {
    // top left corner
    if (currentRow == 0 && currentCol == 0) {
        return CORNER_TOP_LEFT;
    }
    // top right corner
    else if (currentRow == 0 && currentCol == width - 1) {
        return CORNER_TOP_RIGHT;
    }
    // bottom left corner
    else if (currentRow == height - 1 && currentCol == 0) {
        return CORNER_BOTTOM_LEFT;
    }
    // bottom right corner
    else if (currentRow == height - 1 && currentCol == width - 1) {
        return CORNER_BOTTOM_RIGHT;
    }
    // top edge
    else if (currentRow == 0) {
        return EDGE_TOP;
    }
    // bottom edge
    else if (currentRow == height - 1) {
        return EDGE_BOTTOM;
    }
    // left edge
    else if (currentCol == 0) {
        return EDGE_LEFT;
    }
    // right edge
    else if (currentCol == width - 1) {
        return EDGE_RIGHT;
    }
    // middle
    else {
        return MIDDLE;
    }
}

/**
 * Fills the intensity matrix and kernel with zeros depending on their location.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param currentRow current height location of the pixel
 * @param currentCol current width location of the pixel
 * @param currentRGB current red, green, or blue intensity of the pixel
 * @param pix[][][] array of the input image's RGB values
 * @param intensityMatrix[][] intensity matrix of the pixel and its surrounding
 * @param kernel Gaussian distribution kernel
 * @param location current location of the pixel
 */
void prepConvArrays(int width, int height, int currentRow, int currentCol,
        int currentRGB, unsigned char pix[height][width][DEPTH],
        int intensityMatrix[KERNEL_SIZE][KERNEL_SIZE],
        int kernel[KERNEL_SIZE][KERNEL_SIZE], int location) {
    switch (location) {
    // top left corner
    case CORNER_TOP_LEFT:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (i == 0 || j == 0) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    int rowIndex = currentRow + i - 1;
                    int colIndex = currentCol + j - 1;
//                    printf("%d", pix[rowIndex][colIndex][currentRGB]);
                    int valueToAdd = (int) pix[rowIndex][colIndex][currentRGB];
                    intensityMatrix[i][j] = valueToAdd;
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // top right corner
    case CORNER_TOP_RIGHT:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (i == 0 || j == KERNEL_SIZE - 1) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // bottom left corner
    case CORNER_BOTTOM_LEFT:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (i == KERNEL_SIZE - 1 || j == 0) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // bottom right corner
    case CORNER_BOTTOM_RIGHT:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (i == KERNEL_SIZE - 1 || j == KERNEL_SIZE - 1) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // top edge
    case EDGE_TOP:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (i == 0) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // bottom edge
    case EDGE_BOTTOM:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (i == KERNEL_SIZE - 1) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // left edge
    case EDGE_LEFT:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (j == 0) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // right edge
    case EDGE_RIGHT:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill ignored indices with zeros
                if (j == KERNEL_SIZE - 1) {
                    intensityMatrix[i][j] = 0;
                    kernel[i][j] = 0;
                } else {
                    // fill intensity matrix with the pixel and its surroundings
                    intensityMatrix[i][j] = pix[currentRow + i - 1][currentCol
                            + j - 1][currentRGB];
                    // fill the kernel with the Gaussian distribution
                    kernel[i][j] = kernelGaussian[i][j];
                }
            }
        }
        break;

        // middle
    case MIDDLE:
        for (int i = 0; i < KERNEL_SIZE; ++i) {
            for (int j = 0; j < KERNEL_SIZE; ++j) {
                // fill intensity matrix with the pixel and its surroundings
                intensityMatrix[i][j] =
                        pix[currentRow - 1][currentCol - 1][currentRGB];
                // fill the kernel with the Gaussian distribution
                kernel[i][j] = kernelGaussian[i][j];
            }
        }
        break;
    }
}

/**
 * Calculates the resulting intensity value from the convolution using the
 * Gaussian distribution and taking the average of the weighted sum of the
 * pixel with its surroundings.
 *
 * @param height number of rows according to the location of the pixel
 * @param width number of columns according to the location of the pixel
 * @param intensityMatrix[][] intensity matrix of the pixel and its surrounding
 * @param kernel Gaussian distribution kernel
 * @return weighted average of the pixel and its surroundings
 */
unsigned char convolution(int intensityMatrix[KERNEL_SIZE][KERNEL_SIZE],
        int kernel[KERNEL_SIZE][KERNEL_SIZE]) {
    // convolution result
    unsigned char convolutedMess;
    // initialize weighted sum, divisor and declare weighted average
    int weightedSum = 0, divisor = 0, weightedAverage;

    for (int i = 0; i < KERNEL_SIZE; ++i) {
        for (int j = 0; j < KERNEL_SIZE; ++j) {
            // multiply and sum kernel and intensity at each index
            weightedSum += intensityMatrix[i][j] * kernel[i][j];
            // sum all the kernels as the divisor
            divisor += kernel[i][j];
        }
    }

    // calculated weighted average
    weightedAverage = weightedSum / divisor;
    convolutedMess = (unsigned char) weightedAverage;

    return convolutedMess;
}

/**
 * Creates a pixel array with zeros for each pixel's RGB value.
 *
 * @param height height of the image pixels
 * @param width width of the image pixels
 * @param newPix[][][] array of empty RGB values that has the same size as pix
 */
void newPixArray(int height, int width,
        unsigned char newPix[height][width][DEPTH]) {
// row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // each RGB value
            for (int k = 0; k < DEPTH; ++k) {
                // define zero as the value for each kernel
                newPix[i][j][k] = 0;
            }
        }
    }
}

/**
 * Initializes a height x width 2D array to zeros.
 *
 * @param height height of the 2D array
 * @param width width of the 2D array
 */
void zeros(int height, int width, int zeros[height][width]) {
// row/height
    for (int i = 0; i < height; ++i) {
        // column/width/each pixel
        for (int j = 0; j < width; ++j) {
            // define zero as the value for each element
            zeros[i][j] = 0;
        }
    }
}
