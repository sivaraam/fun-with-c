#include <stdio.h>
#include <string.h>
#include "file_lines_offset.h"

int main(void)
{
  char name[100+1];
  printf("Enter the file name (100 chars limit): ");
  fgets(name, 100, stdin);
  name[strcspn(name, "\n")] = '\0';

  struct lines *l = get_line_offsets(name);

  printf("Found %lu lines in the file.\n", l->lines);
  printf("Lines and their offsets:\n");
  for(size_t i=0; i<l->lines; i++)
  {
    printf("Line %zu: %lu\n", i+1, *(l->byte_offsets+i));
  }
}
