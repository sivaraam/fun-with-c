#include <stdio.h>

int main(void) {
	const int a = 5;
	printf("Address of const int before changing: %p\n", &a);
	int *const_ptr = &a;
	printf("Address of const int after assigning address: %p\n", &a);
	*const_ptr = 10;
	printf("Address of const int after changing: %p\n", &a);
}
