#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../file_lines_offset/file_lines_offset.h"

#define LINES 100000
#define FILE_SIZE_BYTES 3915074

int main(void)
{
  char *file_name = "find_op";
  char buf[10000+1];
  FILE *fp = fopen(file_name, "r");

  struct lines *l = get_line_offsets(file_name);

  for (unsigned i=0; i<l->lines; i++)
  {
    unsigned long random_line_byte = *(l->byte_offsets + (rand()%l->lines));

    fseek(fp, random_line_byte, SEEK_SET);
    fscanf(fp, "%[^\n]s", buf);
    // read the new line
    fgetc(fp);

    printf("%s\n", buf);
  }

  printf("\n");
  clock_t c = clock();
  if (c == (clock_t)-1)
  {
      printf("Could not get clock!\n");
  }
  else
  {
    printf("clock: %lf\n", (float)c/CLOCKS_PER_SEC);
  }
}
