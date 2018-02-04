/*
 * MEMORY CRUNCHER
 *
 * **WARNING**: DO NOT execute this without knowing the implications.
 *              You would definitely FEEL for it later if you ignore the
 *              this WARNING.
 *
 * A test to see what happens when we ask for the largest possible memory
 * from the computer and manipulate it sequentially. This causes high memory
 * consumption thus resulting in a large amount of page swaps.
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

int main() {
  size_t alloc_size = SIZE_MAX;
  char *p = NULL;

  // Get the largest possible memory available
  while (p == NULL) {
    printf("Trying allocation of %zu bytes.\n", alloc_size);
    p = malloc(alloc_size);
    if (p == NULL)
      alloc_size /= 2u;
  }
  printf("Allocation of %zu bytes succeeded.\n", alloc_size);

  // Manipulate the obtained memory to ensure you use it
  // after obtaining it.
  //
  // Note: Without manipulating the obtained memory you wouldn't see
  // any signs of obtaining the largest possible memory!
  size_t byte = 0;
  unsigned int rounds = 1;
  while (1) {
    *(p+byte) = rand() % CHAR_MAX;
    byte = (byte+1u) % alloc_size;
    if (byte == 0)
      rounds++;
  }

  printf("Was manipulating the %zuth byte in the %u round", byte, rounds);
}
