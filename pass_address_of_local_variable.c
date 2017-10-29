#include <stdio.h>

void foo(int *a) {
	*a = *a+10;
}

int main(void) {
	int val = 10;

	printf("val before call: %d\n", val);
	foo(&val);                             // "passing" address of local variable to another function is NOT an issue but returning it is
	printf("val after call: %d\n", val);
}
