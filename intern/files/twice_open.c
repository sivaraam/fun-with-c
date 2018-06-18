#include <stdio.h>

int main(void)
{
  FILE *find_op = fopen("find_op", "r");
  FILE *find_op_2 = fopen("find_op", "a");

  if (find_op != NULL && find_op_2 != NULL)
  {
    printf("File 'find_op' opened in 2 distinct modes (r, w) successfully.\n");
    printf("find_op: %p\t find_op_2: %p\n", find_op, find_op_2);
  }
  else if (find_op_2 == NULL && find_op_2 != NULL)
  {
    printf("Opening file 'find_op' for the second time failed.\n");
  }
  else if (find_op == NULL)
  {
    printf("Opening file 'find_op' for first time itself failed!\n");
  }
  else
  {
    printf("All assumptions failed !!!\n");
  }

  fseek(find_op, 0, SEEK_END);
  printf("fprintf() on a file with append mode returned: %d\n", fprintf(find_op_2, "%s", "Hello"));
  printf("fprintf() on a file with read mode returned: %d\n", fprintf(find_op, "%s", "Hello"));

  char buf[5+1];
  fflush(find_op_2);
  fseek(find_op, -5, SEEK_CUR);
  printf("%s", fgets(buf, 5, find_op));
  fclose(find_op);
  fclose(find_op_2);
}
