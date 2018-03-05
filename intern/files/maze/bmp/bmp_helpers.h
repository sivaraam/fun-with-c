#ifndef KS_BMP_HELPERS
#define KS_BMP_HELPERS

#define KS_BMP_HELPERS_DEBUG

static const unsigned bytes_per_pixel = 3;

/**
* Find the number of bytes of possible padding
* for a BMP image given its width.
*/
unsigned find_padding(unsigned width);

#endif
