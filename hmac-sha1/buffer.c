/**
 *	@file buffer.c
 *	@author Timothy Japit
 *	This component is responsible for reading and storing the input file. It uses
 *	a struct named Buffer to store the entire file contents in memory.
 */
#include "buffer.h"
#include "sha1.h"
#include <stdlib.h>
#include <stdio.h>

/** Initial capacity of the buffer. */
#define INITIAL_CAP 64

/**
 * This function dynamically allocates a Buffer struct, initializes its fields
 * (a typical representation for a resizable array).
 *
 * @return pointer to a Buffer struct, container for resizable array of bytes
 */
Buffer* makeBuffer()
{
  // Allocating for the struct
  Buffer *buffer = (Buffer*) malloc(sizeof(Buffer));
  buffer->cap = SHA1_BLOCK;
  buffer->len = 0;

  // Allocating for the data
  buffer->data = (unsigned char*) malloc(buffer->cap * sizeof(unsigned char));

  // Return pointer to buffer
  return buffer;
}

/**
 * This function adds a single byte to the end of the given buffer, enlarging
 * the data array if necessary.
 *
 * @param b pointer to the Buffer struct
 * @param byte unsigned char of a single byte
 */
void appendBuffer(Buffer *b, unsigned char byte)
{
  // Check capacity of buffer
  if ((b->len + 1) > b->cap)
  {
    // Double capacity
    b->cap += SHA1_BLOCK;

    // Reallocate data field
    b->data = (unsigned char*) realloc(b->data,
        b->cap * sizeof(unsigned char*));
  }

  // Append the byte of data
  b->data[b->len] = byte;

  // Increment length
  b->len++;
}

/**
 * This function frees all the memory for the given buffer.
 *
 * @param b pointer to the Buffer struct
 */
void freeBuffer(Buffer *b)
{
  // Free data
  free(b->data);
  // Free struct
  free(b);
}

/**
 * This function creates a new buffer, reads the contents of the file with the
 * given name, stores it in the buffer and returns a pointer to the buffer. It
 * returns NULL if the file can't be opened.
 *
 * @param filename constant char pointer to the filename
 * @return pointer to the Buffer struct
 */
Buffer* readFile(const char *filename)
{
  // Open a pointer to the file
  FILE *fp = fopen(filename, "rb");

  // Error check for the file
  if (!fp)
    return NULL;

  // Create a new buffer
  Buffer *buffer = makeBuffer();

  // Read contents of file in binary in a while loop
  size_t n = sizeof(unsigned char);
  int len;
  unsigned char *byte = (unsigned char*) malloc(sizeof(unsigned char));
  while ((len = fread(byte, n, 1, fp)) != 0)
  {
    // Append the contents of the file to the buffer
    appendBuffer(buffer, *byte);

    // Free byte
    free(byte);

    // Reinitialize byte
    byte = (unsigned char*) malloc(sizeof(unsigned char));
  };

  // Close the file and free any dynamically allocated memory
  fclose(fp);
  free(byte);

  // Return pointer to buffer
  return buffer;
}
