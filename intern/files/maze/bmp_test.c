#include <stdio.h>
#include <stdlib.h>
#define PIXEL_PER_ROW 41

/**
 * Find the number of bytes of possible padding
 * for a BMP image given its width.
 */
unsigned find_padding(unsigned width)
{
  static const unsigned byte_per_pixel = 3, padding_boundary = 4;
  const unsigned row_size = width*byte_per_pixel;

  return padding_boundary-(row_size%4);
}

int main(void)
{
  FILE *fp = fopen("test_inputs/BMP3.bmp", "r");
  unsigned width=0, height=0;

  struct pixel
  {
    unsigned char red;  // expect char to be 8 bits
    unsigned char green;
    unsigned char blue;
  } row[PIXEL_PER_ROW] = {0};

  if (fp == NULL)
  {
    printf("Could not open the file!");
    return 1;
  }

  // get the image width and height

  // go to the offset of the image width
  fseek(fp, 18L, SEEK_SET);
  fread(&width, 4L, 1, fp);
  fread(&height, 4L, 1, fp);
  printf("Image dimensions (in pixels):\n");
  printf("width: %u\t height: %u\n", width, height);

  // find the padding
  unsigned long padding = find_padding(width);
  printf("padding: %u bytes\n", find_padding(width));

  void *padding_bytes = malloc(padding);

  // skip past the header
  fseek(fp, 54L, SEEK_SET);

  printf("sizeof(struct pixel): %zu\n", sizeof(struct pixel));

  for (size_t i=0; i<2; i++)
  {
    printf("Pixel row %zu:\n", i+1);

    // read a row
    fread(row, 3L, PIXEL_PER_ROW, fp);

    // read in the padding
    fread(&padding, padding, 1, fp);

    for (size_t i=0; i<PIXEL_PER_ROW; i++)
    {
      printf("pixel %03zu -> red: %02x, green: %02x, blue: %02x\n", i, row[i].red, row[i].green, row[i].blue);
    }

    printf("\n");
  }
}
