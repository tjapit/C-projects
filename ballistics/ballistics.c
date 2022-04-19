/**
 * @file ballistics.c
 * @author Timothy Japit
 * This program calculates a ballistic trajectory and prints out a table of the
 * trajectory as a function of time.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/** Gravitational acceleration (m/s^2) */
#define GRAVITY 9.81
/** PI */
#define M_PI 3.14159265358979323846
/** Degrees to radian */
#define RAD_CONV M_PI/180
/** Vertical border */
#define BORDER_VERTICAL '|'
/** Horizontal border */
#define BORDER_HORIZONTAL '-'
/** Intersection of the borders */
#define BORDER_INTERSECTION '+'
/** Width of each column in the table */
#define COLUMN_WIDTH 10
/** Minimum angle with respect to the ground */
#define DEG_MIN 0
/** Maximum angle with respect to the ground */
#define DEG_MAX 90
/** Angle increment */
#define DEG_INCREMENT 5
/** Number of header titles */
#define NUM_OF_HEADERS 4

/** Function prototypes */
void repeatChars(char, int);
double flightTime(int, double);
double distanceCalc(int, double, double);
void tableRow(int, double, double);
void tableHeader();

/**
 * Program starting point, reads input from the user, prints the table header,
 * and the rest of the table.
 *
 * @return program exit code
 */
int main() {
    // prompt
    double v0;
    printf("Initial velocity of the projectile (v0): ");
    (void) scanf("%lf", &v0);
    printf("\n");

    // table header
    tableHeader();

    // print table rows
    for (int i = DEG_MIN; i <= DEG_MAX; i += DEG_INCREMENT) {
        tableRow(i, v0, flightTime(i, v0));
    }

    // exit status
    return EXIT_SUCCESS;
}

/**
 * Calculates the time the projectile is in the air from the given angle, and
 * initial velocity.
 *
 * @param angle the direction of the velocity with respect to the ground
 * @param v0 initial velocity
 * @return duration of the flight
 */
double flightTime(int angle, double v0) {
    // convert angle to radians
    double angleRad = angle * RAD_CONV;
    // initial vertical velocity
    double v0y = v0 * sin(angleRad);
    // calculate time to reach highest point
    double t = 0;
    t = 2 * v0y / GRAVITY;
    // return flight time
    return t;
}

/**
 * Calculates the distance traveled by the projectile from the given angle,
 * initial velocity, and flight time.
 *
 * @param angle the direction of the velocity with respect to the ground
 * @param v0 initial velocity
 * @param t duration of the flight
 * @return distance traveled
 */
double distanceCalc(int angle, double v0, double t) {
    // convert angle to radians
    double angleRad = angle * RAD_CONV;
    // horizontal velocity
    double v0x = v0 * cos(angleRad);
    // calculate distance
    double dist = 0;
    dist = v0x * t;
    // return calculated distance
    return dist;
}

/**
 * Prints out each table row which consists of the given angle and initial
 * velocity, as well as the flight time and the projectile travel distance.
 *
 * @param angle the direction of the velocity with respect to the ground
 * @param v0 initial velocity
 * @param t duration of the flight
 */
void tableRow(int angle, double v0, double t) {
    // calculate distance traveled
    double dist = distanceCalc(angle, v0, t);
    // array of doubles to be printed
    double values[NUM_OF_HEADERS - 1] = { v0, t, dist };
    // print rows, fence-post
    printf("%10d ", angle);
    for (int i = 0; i < NUM_OF_HEADERS - 1; i++) {
        printf("%c %10.3lf ", BORDER_VERTICAL, values[i]);
    }
    // new line
    printf("\n");
}

/**
 * Prints out the table header.
 */
void tableHeader() {
    // string array of header titles
    char headerTitles[NUM_OF_HEADERS][COLUMN_WIDTH];
    // string copy
    strcpy(headerTitles[0], "angle");
    strcpy(headerTitles[1], "v0");
    strcpy(headerTitles[2], "time");
    strcpy(headerTitles[3], "distance");

    // print first header, fence-post
    printf("%10s ", headerTitles[0]);
    // print the rest of the headers
    for (int i = 1; i < NUM_OF_HEADERS; i++) {
        printf("%c %10s ", BORDER_VERTICAL, headerTitles[i]);
    }
    // new line
    printf("\n");

    // title bottom border, fence-post
    repeatChars(BORDER_HORIZONTAL, COLUMN_WIDTH + 1);
    for (int i = 1; i < NUM_OF_HEADERS - 1; i++) {
        printf("%c%c", BORDER_INTERSECTION, BORDER_HORIZONTAL);
        repeatChars(BORDER_HORIZONTAL, COLUMN_WIDTH + 1);
    }
    printf("%c", BORDER_INTERSECTION);
    repeatChars(BORDER_HORIZONTAL, COLUMN_WIDTH + 1);

    // new line
    printf("\n");

}

/**
 * Prints out a line of the given character to the number of repetition,
 * without a new line.
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
}
