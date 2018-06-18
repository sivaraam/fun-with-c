#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LINES 100000

extern char find[][1000];

int main(void)
{
  for (size_t i=0; i<LINES;i++)
  {
    size_t index = rand()%LINES;
    printf("%s\n", find[index]); //fwrite(find[i], 100, 1, stdout);
  }

  printf("\n");
  clock_t c = clock();
  if (c == (clock_t)-1)
  {
      printf("Could not get clock!\n");
  }
  else
  {
    printf("CPU time: %lf\n", (float)c/CLOCKS_PER_SEC);
  }
}
