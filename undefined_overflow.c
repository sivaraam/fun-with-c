// For gcc overflows are undefined in -O2 as the
// -fstrict-overflow option is enabled

#include <stdio.h>
#include <limits.h>

int main() {
  int i=INT_MAX-100;
  short s=0;
  char c=CHAR_MAX;

  s = SHRT_MAX-9;
  i = INT_MAX-9;

  // i+10 overflows in 2's complement arithmetic
  printf("%hd > %hd %d\n", s+10, s, s+10>s); // prints 1 where i is a short (why ????)
  printf("%d > %d %d\n", i+10, i, i+10>i); // prints 0 where i is an int

  // the loop never terminates  with -O2 or -fstrict-overflow
  for (; i+10>i; i++) {
    printf("Int Looping.. %d\n", i);
  }

  // the loop terminates regardless of -fstrict-overflow
  for (; c>=10; c++) {
    printf("Char Looping.. %d\n", c);
  }

  printf("Int loop terminated %d\n", i);
  printf("Char Loop terminated %d\n", c);

}
