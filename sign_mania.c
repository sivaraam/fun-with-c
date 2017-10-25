// Should do a lot of tests that play around with
// signed and unsigned numbers to see what happens
// when one type is used in place of the other.

#include <stdio.h>

int main(void) {
	unsigned u_int;
	int i;

	printf("Sign Mania\n");

	printf("\nTry intterpreting a signed number as an unsigned into an unsigned\n");
	scanf("%u", &u_int);
	printf("Consequence: %u\n", u_int);

	printf("\nTry intterpreting a signed number as an unsigned into a signed\n");
	scanf("%u", &i);
	printf("Consequence: %d %u\n", i, i);
}
