#include <stdio.h>

int main(void)
{
  char buf[100+1];
  FILE *fp = fopen("find_op", "r");
  unsigned long round = 1;

  while(!feof(fp))
  {
    fprintf(stderr, "Successfully read '%zu' item in round: %lu\n", fread(buf, 100, 1, fp), round);
    fwrite(buf, 100, 1, stdout);
//    printf("%s", buf);
    fflush(stdout);
    round++;
  }
}
