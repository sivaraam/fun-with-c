#include <stdio.h>
#include <stdlib.h>
#include "file_lines_offset.h"

// #define DEBUG

#ifdef DEBUG
#endif

/**
 *  For now assume multiplication doesn't overflow.
 *  FIXME: Check for overflows.
 *
 *  Returns 0 on success and non-zero value on failure.
 */
int re_allocate(unsigned long **line_buf, size_t size, size_t nmemb)
{
  unsigned long *temp = realloc(*line_buf, size*nmemb);
  if (temp != NULL)
  {
    *line_buf = temp;
    temp = NULL;
    return 0;
  }
  else
  {
    return 1;
  }
}

struct lines *get_line_offsets(char *file_name)
{
  FILE *fp = fopen(file_name, "r");

  if (fp == NULL)
  {
    return NULL;
  }

  const size_t initial_size = 25, increment = initial_size;
  size_t curr_limit = initial_size;
  struct lines *l = malloc(sizeof(struct lines));
  l->byte_offsets = NULL;
  unsigned long line = 0, position = 0;

  // initial allocation. assume it won't fail.
  re_allocate(&(l->byte_offsets), sizeof(unsigned long), initial_size);

  while (1)
  {
    *(l->byte_offsets+line) = position;

    // get the next line to find the position of the
    // lline that follows
    int ch = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF);

    if (ch == EOF)
    {
      break;
    }

    line++;
    position = ftell(fp);

    if(line>=curr_limit)
    {
      const size_t new_limit = curr_limit+increment;
      if (re_allocate(&(l->byte_offsets), sizeof(unsigned long), new_limit))
      {

#ifdef DEBUG
        // FIXME: handle this in a better way
        fprintf(stderr, "Could not get more memory to specify line offsets.");
        printf("realloc failed for new_limit: %zu\n", new_limit);
#endif
        exit(EXIT_FAILURE);
      }
      else
      {
        curr_limit = new_limit;
      }
    }
  }

  // There would be an entry for the last byte of the file
  // as the file is expected to end with a new line character
  //
  // Hide that entry by not adding 1 to 'line' below.
  l->lines = line;
  return l;
}
