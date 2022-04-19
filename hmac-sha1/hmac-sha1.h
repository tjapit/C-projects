/**
 *  @file hmac-sha1.h
 *  @author Timothy Japit
 *  This component defines a single utility hmacSHA1() for performing the HMAC
 *  operation for a given key and input.
 */

#ifndef HMAC_SHA1_H_
#define HMAC_SHA1_H_

#include "sha1.h"
#include "buffer.h"

/**
 * This function performs the HMAC-SHA1 by taking a key as a string of
 * characters, a pointer to a Buffer struct, and a pointer to an area of memory
 * to store the digest (using sha1Encode()).
 *
 * @param kstr the key from user input
 * @param b pointer to the Buffer struct
 * @param digest placeholder for the digest
 */
void hmacSHA1(char *kstr, Buffer *b, unsigned char digest[SHA1_DIGEST]);


#endif /* HMAC_SHA1_H_ */
