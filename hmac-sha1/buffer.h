/**
 * @file buffer.c
 * @author Timothy Japit
 * This component is responsible for reading and storing the input file. It uses
 * a struct named Buffer to store the entire file contents in memory.
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

/** Representation for the contents of an input file, copied to memory. */
typedef struct
{
  /** Array of bytes from the file (not stored as a string - no NULL termination). */
  unsigned char *data;

  /** Number of currently used bytes in the data array. */
  unsigned int len;

  /** Capacity of the data array (it's typically over-allocated. */
  unsigned int cap;
} Buffer;


/**
 * This function dynamically allocates a Buffer struct, initializes its fields
 * (a typical representation for a resizable array).
 *
 * @return pointer to a Buffer struct, container for resizable array of bytes
 */
Buffer* makeBuffer();

/**
 * This function adds a single byte to the end of the given buffer, enlarging
 * the data array if necessary.
 *
 * @param b pointer to the Buffer struct
 * @param byte unsigned char of a single byte
 */
void appendBuffer(Buffer *b, unsigned char byte);

/**
 * This function frees all the memory for the given buffer.
 *
 * @param b pointer to the Buffer struct
 */
void freeBuffer(Buffer *b);

/**
 * This function creates a new buffer, reads the contents of the file with the
 * given name, stores it in the buffer and returns a pointer to the buffer. It
 * returns NULL if the file can't be opened.
 *
 * @param filename constant char pointer to the filename
 * @return pointer to the Buffer struct
 */
Buffer* readFile(const char *filename);

#endif
