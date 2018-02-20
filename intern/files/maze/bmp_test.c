#include <stdio.h>
#include <stdlib.h>
#include "bmp/bmp_helpers.h"

#define DEBUG

#ifdef DEBUG
/**
 * Checks whether the given image data size matches the value of
 * '<file_size> - <header_size>'.
 *
 * Returns 0 on success and non-zero value on failure.
 */
int check_image_data_size(FILE *image_file, unsigned long image_header_size, unsigned long image_data_size)
{
  long old_pos = ftell(image_file); // save old position

  fseek(image_file, 0, SEEK_END);
  const long file_size = ftell(image_file);
  if (file_size-image_header_size == image_data_size)
  {
    printf("Value of image data size matches <file_size>-<header_size>.\n");
    fseek(image_file, old_pos, SEEK_SET);
    return 0;
  }
  else
  {
    fprintf(stderr, "Image data size didn't match <file_size>-<header_size>!\n");
    return 1;
  }

}
#endif

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("File name is required as first argument!\n");
    return 1;
  }
  else if (argc > 2)
  {
    printf("Too many arguments.\n");
    return 1;
  }

  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL)
  {
    printf("Could not open the file!");
    return 1;
  }

  static const unsigned long width_offset = 18L;
  unsigned width=0, height=0;
  unsigned char *image = NULL;

  // get the image width and height
  fseek(fp, width_offset, SEEK_SET); // go to the offset of the image width
  fread(&width, 4L, 1, fp);
  fread(&height, 4L, 1, fp);
  printf("Image dimensions (in pixels):\n");
  printf("width: %u\t height: %u\n", width, height);

  // find the padding
  unsigned long padding = find_padding(width);
  printf("padding: %lu bytes\n", padding);

  // find the image data size
  unsigned long image_data_size = (height*width*bytes_per_pixel)+(padding*height);
  printf("Size of image data: %lu\n", image_data_size);

#ifdef DEBUG
  if (check_image_data_size(fp, 54L, image_data_size))
  {
    fprintf(stderr, "Assumption failed!\n");
    return 1;
  }
#endif

  // allocate memory to read in the image
  image = malloc(image_data_size);

  if (image == NULL)
  {
    fprintf(stderr, "Not enough memory to read in image!\n");
    exit(EXIT_FAILURE);
  }

  // skip past the header
  fseek(fp, 54L, SEEK_SET);

  // read the image
  if (fread(image, image_data_size, 1, fp) == 0)
  {
    fprintf(stderr, "Reading image failed!\n");
    exit(EXIT_FAILURE);
  }

#ifdef DEBUG
  unsigned row = 1;
  for (unsigned long pixel=0; pixel<width*height; pixel++)
  {
    if (pixel%width == 0)
    {
      printf("\n");
    }
/*  if (pixel%width == 0)
    {
      printf("Row %u:\n", row++);
    }
*/
    char *pixel_ptr = get_pixel(image, width, padding, pixel);

    if ((*pixel_ptr&0xFF) == 0xFF)
    {
      printf(" ");
    }
    else if ((*pixel_ptr&0x00) == 0x00)
    {
      printf("\u2588");
    }
    else
    {
      fprintf(stderr, "Unexpected Hex!!\n");
      exit(EXIT_FAILURE);
    }
//    printf("Pixel %04lu -> red: %x, green: %02x, blue: %02x\n", pixel, *pixel_ptr, *(pixel_ptr+1), *(pixel_ptr+2));
  }
  printf("\n");
#endif
}
