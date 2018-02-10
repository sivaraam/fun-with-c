#include <stdio.h>
#include <stdlib.h>

int main(void) {
//	char *p = (char*)0x10000000000;
	char *p = (char*)malloc(1);
	*p  = 'a';
	*(p+1) = 'b';  // what's wrong with doing this?
	printf("%p\n", p);
	printf("%c %c\n", *p, *(p+1));
}
