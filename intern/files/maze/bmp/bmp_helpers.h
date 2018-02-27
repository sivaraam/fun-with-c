#ifndef KS_BMP_HELPERS
#define KS_BMP_HELPERS

#define KS_BMP_HELPERS_DEBUG

static const unsigned bytes_per_pixel = 3;

/**
* Find the number of bytes of possible padding
* for a BMP image given its width.
*/
unsigned find_padding(unsigned width);

/**
 * Given the image width and the amount of padding in it gives the byte offset
 * for Nth pixel. (Indexed from 0).
 *
 * width - width of the image
 * padding - the number of bytes of padding between the rows (cannot exeeed 3)
 * N - the pixel required
 *     assumed to be in the range of width*height of the image
 */
unsigned long get_pixel_offset(unsigned long width, unsigned char padding, unsigned long N);

#endif
