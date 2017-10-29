#include <stdio.h>

int main(void) {
	size_t a = 10;
	int arr[10];
	size_t b = 20;

	printf("\nAddress of a: %p\n", &a);
	printf("sizeof a: %zu\n", sizeof(a));

	printf("\nAddress IN arr: %p\n", arr);
	printf("Address OF arr: %p\n", &arr);
	printf("sizeof arr: %zu\n", sizeof(arr));

	printf("\nAddress of b: %p\n", &b);
	printf("sizeof b: %zu\n", sizeof(b));
}
