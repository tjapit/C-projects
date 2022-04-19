/**
 * @file sha1.c
 * @author Timothy Japit
 * This component contains a struct to store the SHA1 state and functions to
 * compute various parts of the SHA1 algorithm.
 */

#include "sha1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Constants, to mix in some random-looking bits during the SHA1
 calculation.  80 constants for 80 iterations.  The constants for one
 round (iterations 0-19, 20-39, 40-59, 60-79) all have the same value. **/
static unsigned int k[SHA1_ITER] =
{ 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
    0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
    0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
    0x5A827999, 0x5A827999, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
    0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
    0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
    0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x8F1BBCDC, 0x8F1BBCDC,
    0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
    0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
    0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
    0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
    0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
    0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
    0xCA62C1D6, 0xCA62C1D6, };

/**
 * Given the address of a SHA1State, this function initializes its fields,
 * filling them in with the five constant values given in the SHA1 algorithm.
 *
 * @param state pointer to the SHA1State
 */
void initState(SHA1State *state)
{
  // Initializing states to given pre-defined constant
  state->h0 = 0x67452301;
  state->h1 = 0xEFCDAB89;
  state->h2 = 0x98BADCFE;
  state->h3 = 0x10325476;
  state->h4 = 0xC3D2E1F0;
}

/**
 * This function pads the given buffer, bringing its length up to a multiple of
 * 64 bytes, adding byte values as described in the SHA1 algorithm.
 *
 * @param b pointer to the buffer
 */
void padBuffer(Buffer *b)
{
  // Store message length in number of bits
  unsigned long msglen = b->len * BYTES_MSG;

  // Add the 0x80
  appendBuffer(b, 0x80);

  // CHECK IF THERE'S ENOUGH ROOM TO PAD FOR LENGTH

  // Pad with 0s
  for (int i = b->len; i < (b->cap - BYTES_MSG); ++i)
  {
    appendBuffer(b, 0x00);
  }

  // Append message length
  for (int i = 0; i < BYTES_MSG; ++i)
  {
    // Number of shifts of a 64-bit (8 bytes) long
    int shift = (b->cap - BYTES_MSG) - (BYTES_MSG * i);

    // Shifting the long into 8-bit (1 byte) character
    unsigned char app = msglen >> shift; // credit to Bruno Volpato

    // Appending length byte-by-byte
    appendBuffer(b, app);
  }
}

/**
 * Version 1 function for the SHA1 algorithm, from iteration 1 to 19.
 *
 * @param b SHA1 internal state h1
 * @param c SHA1 internal state h2
 * @param d SHA1 internal state h3
 * @return result of the computation
 */
test_static unsigned int fVersion0(unsigned int b, unsigned int c,
    unsigned int d)
{
  return (b & c) | (~b & d);
}

/**
 * Version 2 function for the SHA1 algorithm, from iteration 20 to 39.
 *
 * @param b SHA1 internal state h1
 * @param c SHA1 internal state h2
 * @param d SHA1 internal state h3
 * @return result of the computation
 */
test_static unsigned int fVersion1(unsigned int b, unsigned int c,
    unsigned int d)
{
  return b ^ c ^ d;
}

/**
 * Version 3 function for the SHA1 algorithm, from iteration 40 to 59.
 *
 * @param b SHA1 internal state h1
 * @param c SHA1 internal state h2
 * @param d SHA1 internal state h3
 * @return result of the computation
 */
test_static unsigned int fVersion2(unsigned int b, unsigned int c,
    unsigned int d)
{
  return (b & c) | (b & d) | (c & d);
}

/**
 * Version 4 function for the SHA1 algorithm, from iteration 60 to 79.
 *
 * @param b SHA1 internal state h1
 * @param c SHA1 internal state h2
 * @param d SHA1 internal state h3
 * @return result of the computation
 */
test_static unsigned int fVersion3(unsigned int b, unsigned int c,
    unsigned int d)
{
  return b ^ c ^ d;
}

/**
 * This function implements the rotate left operation from the SHA1 algorithm,
 * shifting the given value to the left by s bits, with a wrap around.
 *
 * @param value value to be rotated
 * @param s number of bit shifts
 * @return left rotated value
 */
test_static unsigned int rotateLeft(unsigned int value, int s)
{
  return (value << s) | (value >> (UINT_BITS - s));
}

/**
 * This function implements an iteration of the SHA1 algorithm on a 64-byte
 * block (interpreted as 16 unsigned integers).
 *
 * @param data block of 16 unsigned integers to be processed
 * @param a SHA1 internal state h0
 * @param b SHA1 internal state h1
 * @param c SHA1 internal state h2
 * @param d SHA1 internal state h3
 * @param e SHA1 internal state h4
 * @param i iteration number
 */
test_static void sha1Iteration(unsigned int data[16], unsigned int *a,
    unsigned int *b, unsigned int *c, unsigned *d, unsigned int *e, int i)
{
  // Initializing w
  unsigned int w[SHA1_ITER];
  for (int i = 0; i < 16; ++i)
  {
    w[i] = data[i];
  }
  // Extending the sixteen words to eighty
  for (int i = 16; i < SHA1_ITER; ++i)
  {
    w[i] = rotateLeft(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
  }

  // main iteration
  unsigned int temp;
  // 0-19
  if (i >= 0 && i <= 19)
  {
    temp = *e + rotateLeft(*a, 5) + w[i] + k[i] + fVersion0(*b, *c, *d);
  }

  // 20-39
  else if (i >= 20 && i <= 39)
  {
    temp = *e + rotateLeft(*a, 5) + w[i] + k[i] + fVersion1(*b, *c, *d);
  }

  // 40-59
  else if (i >= 40 && i <= 59)
  {
    temp = *e + rotateLeft(*a, 5) + w[i] + k[i] + fVersion2(*b, *c, *d);
  }

  // 60-79
  else
  {
    temp = *e + rotateLeft(*a, 5) + w[i] + k[i] + fVersion3(*b, *c, *d);
  }
  *e = *d;
  *d = *c;
  *c = rotateLeft(*b, 30);
  *b = *a;
  *a = temp;
}

/**
 * This function is used to create the final hash value. It transfers the 20
 * bytes in the h0, h1, h2, h3, and h4 state variables into a 20 byte unsigned
 * char array, in big-endian order.
 *
 * @param digest final hash value
 * @param state given SHA1 internal state
 */
void sha1Encode(unsigned char digest[SHA1_DIGEST], SHA1State *state)
{
  for (int i = 0; i < SHA1_STATES; ++i)
  {
    unsigned char *currentState;
    // 0-3
    if (i == 0)
    {
      currentState = (unsigned char*) &state->h0;
    }
    // 4-7
    else if (i == 1)
    {
      currentState = (unsigned char*) &state->h1;
    }
    // 8-11
    else if (i == 2)
    {
      currentState = (unsigned char*) &state->h2;
    }
    // 12-15
    else if (i == 3)
    {
      currentState = (unsigned char*) &state->h3;
    }
    // 16-19
    else
    {
      currentState = (unsigned char*) &state->h4;
    }

    // Place bytes (weird, when printed, states shows big-endian, when casted, showed little-endian in currentState)
    digest[0 + (i * BYTES_STATE)] = currentState[3];
    digest[1 + (i * BYTES_STATE)] = currentState[2];
    digest[2 + (i * BYTES_STATE)] = currentState[1];
    digest[3 + (i * BYTES_STATE)] = currentState[0];
  }
}

/**
 * This function performs 80 SHA1 iterations on the given block of bytes,
 * updating the given state.
 *
 * @param data block of 64 bytes to be processed
 * @param state given SHA1 internal state
 */
void sha1Block(unsigned char data[SHA1_BLOCK], SHA1State *state)
{
  // cast unsigned char to unsigned int
  unsigned int *w = (unsigned int*) data;

  // hash value for this block
  unsigned int a, b, c, d, e;
  a = state->h0;
  b = state->h1;
  c = state->h2;
  d = state->h3;
  e = state->h4;

  // Iterate 80 times
  for (int i = 0; i < SHA1_ITER; ++i)
  {
    sha1Iteration(w, &a, &b, &c, &d, &e, i);
  }

  // Add the hash values to the internal state
  state->h0 += a;
  state->h1 += b;
  state->h2 += c;
  state->h3 += d;
  state->h4 += e;

}

/**
 * Converts the Endianness of a any number of byte blocks multiple of 4.
 *
 * @param before before block
 * @param after after block
 */
void endianconv(int blocks, unsigned char before[blocks],
    unsigned char after[blocks])
{
  // Converting every 4 bytes from the 64-byte block
  int numOfConv = blocks / 4;

  for (int i = 0; i < numOfConv; ++i)
  {
    after[3 + (i * 4)] = before[0 + (i * 4)];
    after[2 + (i * 4)] = before[1 + (i * 4)];
    after[1 + (i * 4)] = before[2 + (i * 4)];
    after[0 + (i * 4)] = before[3 + (i * 4)];
  }
}

/**
 * This function performs SHA1 on a message block that has already been padded
 * to a multiple of 64-byte blocks.
 *
 * @param b pointer to the Buffer struct
 * @param digest placeholder for the 20-byte digest
 * @param state pointer to the current SHA1 state
 */
void SHA1(Buffer *b, unsigned char digest[SHA1_DIGEST], SHA1State * state)
{
  // Iterate block by block of 64 bytes
  int numOfBlocks = b->cap / SHA1_BLOCK;
  unsigned char bigE[SHA1_BLOCK], lilE[SHA1_BLOCK];
  for (int i = 0; i < numOfBlocks; ++i)
  {
    // Copy a block of 64 byte message
    memcpy(bigE, b->data + (i * SHA1_BLOCK), SHA1_BLOCK);

    // Convert Endianness
    endianconv(SHA1_BLOCK, bigE, lilE);

    // Calculate the internal state
    sha1Block(lilE, state);
  }
  // Encode to 20-byte (40 unsigned char) digest and print
  sha1Encode(digest, state);
}
