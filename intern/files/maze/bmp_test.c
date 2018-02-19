#include <stdio.h>
#define PIXEL_PER_ROW 41
int main(void)
{
  FILE *fp = fopen("test_inputs/BMP3.bmp", "r");
  unsigned width=0, height=0;

  struct pixel
  {
    unsigned red:8;
    unsigned green:8;
    unsigned blue:8;
  } row[PIXEL_PER_ROW+20] = {0};

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

  // skip past the header
  fseek(fp, 54L, SEEK_SET);

  printf("sizeof(struct pixel): %zu\n", sizeof(struct pixel));

  // read a row
  fread(row, 3L, PIXEL_PER_ROW+20, fp);
  for (size_t i=0; i<PIXEL_PER_ROW+20; i++)
  {
    printf("pixel %zu -> red: %x, green: %x, blue: %x\n", i, row[i].red, row[i].green, row[i].blue);
  }
}
