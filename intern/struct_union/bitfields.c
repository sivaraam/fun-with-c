#include <stdio.h>

struct
{
  unsigned int val:16;
} obj;

int main(void)
{
  unsigned int a[] = { 0x41424344, 0x45464748, 0x49505152};

  for (size_t i=0; i<sizeof(a)/sizeof(int); i++)
  {
    obj.val = a[i];
    printf("0x%x\t", obj.val);
  }
  printf("\n");
}
