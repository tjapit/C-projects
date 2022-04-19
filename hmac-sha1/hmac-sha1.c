/**
 * @file hmac-sha1.c
 * @author Timothy Japit
 * This component defines a single utility hmacSHA1() for performing the HMAC
 * operation for a given key and input.
 */
#include "hmac-sha1.h"
#include <stdlib.h>
#include <string.h>

/**
 * This function pads the given key in the buffer, bringing its length up to a
 * multiple of 64 bytes by padding with 0x00.
 *
 * @param b pointer to the key in a buffer
 */
void padKey(Buffer *b)
{
  // Pad with 0s
  for (int i = b->len; i < b->cap; ++i)
  {
    appendBuffer(b, 0x00);
  }
}

/**
 * This function performs the HMAC-SHA1 by taking a key as a string of
 * characters, a pointer to a Buffer struct, and a pointer to an area of memory
 * to store the digest (using sha1Encode()).
 *
 * @param kstr the key from user input
 * @param b pointer to the Buffer struct containing the message
 * @param digest placeholder for the digest
 */
void hmacSHA1(char *kstr, Buffer *b, unsigned char digest[SHA1_DIGEST])
{
  // Initialize state
  SHA1State *state = (SHA1State*) malloc(sizeof(SHA1State));
  initState(state);

  // Create new buffer to hold inner pad + original message
  Buffer *newb = makeBuffer();

  // Create digest for first round of SHA1
  unsigned char digestR1[SHA1_DIGEST];

  // Put the key string in a unsigned char array
  unsigned char key[SHA1_BLOCK];
  for (int i = 0; i < strlen(kstr); ++i)
  {
    key[i] = kstr[i];
  }

  // Pad key with zeros
  for (int i = strlen(kstr); i < SHA1_BLOCK; ++i)
  {
    key[i] = 0x00;
  }

  // XOR the key with the 512-bit constants to get ipad and opad
  unsigned char ipad[SHA1_BLOCK], opad[SHA1_BLOCK];
  for (int i = 0; i < SHA1_BLOCK; ++i)
  {
    // key XOR inner padding constant (0x36...)
    ipad[i] = key[i] ^ 0x36;

    // key XOR outer padding constant (0x5c...)
    opad[i] = key[i] ^ 0x5c;
  }

  /** Inner Pad, 1st round of SHA1 */
  // Concatenate ipad to the new buffer
  for (int i = 0; i < SHA1_BLOCK; ++i)
  {
    appendBuffer(newb, ipad[i]);
  }
  // Concatenate the original message to the new buffer
  for (int i = 0; i < b->len; ++i)
  {
    appendBuffer(newb, b->data[i]);
  }
  // Pad the new buffer (0x80, 0x00..., bits length in hex)
  padBuffer(newb);

  // 1st round of SHA1
  SHA1(newb, digestR1, state);

  /** Outer Pad, 2nd round of SHA1 */
  // Create another new buffer to hold opad + digest from SHA1 round 1
  Buffer *newb2 = makeBuffer();

  // Concatenate opad to the new new buffer
  for (int i = 0; i < SHA1_BLOCK; ++i)
  {
    appendBuffer(newb2, opad[i]);
  }
  // Concatenate digest to the new new buffer
  for (int i = 0; i < SHA1_DIGEST; ++i)
  {
    appendBuffer(newb2, digestR1[i]);
  }
  // Pad the new new buffer
  padBuffer(newb2);

  // Re-initialize the SHA1 state for the new new buffer
  // (would've been nice to know that we need to re-initialize the state in the project description)
  free(state);
  state = (SHA1State*) malloc(sizeof(SHA1State));
  initState(state);

  // 2nd round of SHA1
  SHA1(newb2, digest, state);

  // Free allocated memory
  freeBuffer(newb);
  freeBuffer(newb2);
  free(state);
}
