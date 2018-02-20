#ifndef KS_BMP_HELPERS
#define KS_BMP_HELPERS

static const unsigned bytes_per_pixel = 3;

/**
* Find the number of bytes of possible padding
* for a BMP image given its width.
*/
unsigned find_padding(unsigned width);

/**
 * Given the image data and the amount of padding in it gives a pointer
 * to the location of the Nth pixel. (Indexed from 0).
 *
 * image_data - the image with/without padding in between the rows
 * padding - the number of bytes of padding between the rows (cannot exeeed 3)
 * width - width of the image
 * unsigned long N - the pixel required
 */
char *get_pixel(char *image_data, unsigned long width, unsigned char padding, unsigned long N);

#endif
