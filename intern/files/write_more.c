#include <stdio.h>

int main(void)
{
  char ch = 'c';

  fwrite(&ch, 100, 1, stdout);
}
