#include "bmp_helpers.h"

#ifdef KS_BMP_HELPERS_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

inline
unsigned find_padding(unsigned width)
{
  static const unsigned padding_boundary = 4;
  const unsigned row_size = width*bytes_per_pixel;

  return padding_boundary-(row_size%4);
}

inline
unsigned long get_pixel_offset(unsigned long width, unsigned char padding, unsigned long N)
{

#ifdef KS_BMP_HELPERS_DEBUG
  if (padding>3)
  {
    fprintf(stderr, "Invalid padding value: %u!", padding);
    exit(EXIT_FAILURE);
  }
#endif

  return N*bytes_per_pixel+((N/width)*padding);
}
