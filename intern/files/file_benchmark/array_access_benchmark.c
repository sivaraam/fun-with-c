#include <stdio.h>

extern char find[][1000];

int main(void)
{
  for (size_t i=0; i<100000;i++)
  {
    printf("%s\n", find[i]); //fwrite(find[i], 100, 1, stdout);
  }
}
