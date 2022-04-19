/**
 * @file sha1.h
 * @author Timothy Japit
 * This component contains a struct to store the SHA1 state and functions to
 * compute various parts of the SHA1 algorithm.
 */

#ifndef _SHA1_H_
#define _SHA1_H_

#include "buffer.h"

/* Mechanism to conditionally expose static functions to other components.  For
 production, we can make make them static, but for testing we can disable
 the static keyword and expose functions to the test driver. */
#ifdef TESTABLE
#define test_static
#else
#define test_static static
#endif

/** Number of bytes in a block used in the SHA1 calculation. */
#define SHA1_BLOCK 64

/** Number of bytes in an SHA1 digest */
#define SHA1_DIGEST 20

/** Number of bytes for length of message */
#define BYTES_MSG 8

/** Number of bytes in a single SHA1 state */
#define BYTES_STATE 4

/** Number of iterations in the SHA1 algorithm */
#define SHA1_ITER 80

/** Number of bits in the unsigned int type */
#define UINT_BITS 32

/** Number of bits in a byte */
#define BYTE_BITS 8

/** Number of states in SHA1 */
#define SHA1_STATES 5

/** Representation for the state of the SHA1 computation.  It's just 5
 unsigned 32-bit integers. Client code can create an instance
 (statically, on the stack or on the heap), but initState() needs
 to initialize it before it can be used. */
typedef struct
{
  /** Fields h0 through h4 of the SHA1 state.  */
  unsigned int h0;
  unsigned int h1;
  unsigned int h2;
  unsigned int h3;
  unsigned int h4;
} SHA1State;

/**
 * Given the address of a SHA1State, this function initializes its fields,
 * filling them in with the five constant values given in the SHA1 algorithm.
 *
 * @param state pointer to the SHA1State
 */
void initState(SHA1State *state);

/**
 * This function pads the given buffer, bringing its length up to a multiple of
 * 64 bytes, adding byte values as described in the SHA1 algorithm.
 *
 * @param b pointer to the buffer
 */
void padBuffer(Buffer *b);

/**
 * This function performs 80 SHA1 iterations on the given block of bytes,
 * updating the given state.
 *
 * @param data block of 64 bytes to be processed
 * @param state given SHA1 internal state
 */
void sha1Block(unsigned char data[SHA1_BLOCK], SHA1State *state);

/**
 * This function is used to create the final hash value. It transfers the 20
 * bytes in the h0, h1, h2, h3, and h4 state variables into a 20 byte unsigned
 * char array, in big-endian order.
 *
 * @param digest final hash value
 * @param state given SHA1 internal state
 */
void sha1Encode(unsigned char digest[SHA1_DIGEST], SHA1State *state);

/**
 * This function performs SHA1 on a message block that has already been padded
 * to a multiple of 64-byte blocks.
 *
 * @param b pointer to the Buffer struct
 * @param digest placeholder for the 20-byte digest
 * @param state pointer to the current SHA1 state
 */
void SHA1(Buffer *b, unsigned char digest[SHA1_DIGEST], SHA1State * state);

/**
 * Converts the Endianness of any number of byte blocks multiple of 4.
 *
 * @param before before block
 * @param after after block
 */
void endianconv(int blocks, unsigned char before[blocks],
    unsigned char after[blocks]);
#endif
