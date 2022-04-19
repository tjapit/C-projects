/**
 *  @file hash.c
 *  @author Timothy Japit
 *  Main component that is responsible for parsing the command-line arguments and
 *  using the other components to read the input file, to perform the SHA1
 *  computation and to compute the HMAC when requested.
 */
#include "buffer.h"
#include "sha1.h"
#include "hmac-sha1.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Prints out an error message if the user runs the command with invalid command-
 * line arguments.
 */
static void usage()
{
  fprintf(stderr, "usage: hash [-hmac <key>] <filename>\n");
  exit(EXIT_FAILURE);
}

/**
 * Prints out an error message if the command-line arguments are valid but the
 * given input file can't be opened.
 *
 * @param filename pointer to the filename
 */
static void badfile(const char *filename)
{
  fprintf(stderr, "Can't open file: %s\n", filename);
  exit(EXIT_FAILURE);
}

/**
 * Program starting point.
 *
 * @param argc number of arguments from the command-line
 * @param argv String array of the arguments from the user
 * @return exit status code
 */
int main(int argc, char *argv[])
{
  // Error check for user arguments
  if ((argc > 2 && strcmp(argv[1], "-hmac") != 0) || argc < 2)
  {
    usage();
  }

  Buffer *buffer;
  unsigned char digest[SHA1_DIGEST];
  // Without HMAC
  if (argc == 2)
  {
    // Read file into buffer
    buffer = readFile(argv[1]);

    // File error check
    if (!buffer)
    {
      badfile(argv[1]);
    }

    // Initialize state
    SHA1State *state = (SHA1State*) malloc(sizeof(SHA1State));
    initState(state);

    // Pad the buffer
    padBuffer(buffer);

    // SHA1 computation
    SHA1(buffer, digest, state);

    // Free allocated memory
    free(state);
  }

  // With HMAC
  else
  {
    // Read file into buffer
    buffer = readFile(argv[3]);

    // File error check
    if (!buffer)
      badfile(argv[3]);

    // HMAC-SHA1
    hmacSHA1(argv[2], buffer, digest);
  }

  // Print the digest
  for (int i = 0; i < SHA1_DIGEST; ++i)
  {
    printf("%02X", digest[i]);
  }
  printf("\n");
  // Free buffer and other dynamically allocated memories
  free(buffer);

  // Exit
  return (EXIT_SUCCESS);
}
