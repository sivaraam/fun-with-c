#include <stdio.h>
#include <string.h>

int main(void) {
  printf("%zu", strlen(NULL)); // Results in 'Segmentation Fault'
}
