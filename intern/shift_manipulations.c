#include <stdio.h>
#include <limits.h>

void unsigned_left_shift_test(char c_orig_val) {
	unsigned char uc_val = c_orig_val;
	printf("Assign %d to unsigned char: %hhu\n", c_orig_val, uc_val);
	uc_val<<=1;
	printf("Left shift it by 1: %hhu\n", uc_val);
	// left shifting by CHAR_BIT-2 results in 'uc_val' having only one bit set. The bit set is the MSB (not the sign bit).
	uc_val<<=CHAR_BIT-2;
	printf("Then left shift it by CHAR_BIT-2: %hhu\n", uc_val);
	uc_val<<=1; // This should discard the only set bit (1) left
	printf("Then left shift it by 1: %hhu\n", uc_val);
	uc_val<<=1; // See what happens when we shift once more
	printf("Finally left shift it by 1: %hhu\n", uc_val);
	printf("\n");
}

void unsigned_right_shift_test(char c_orig_val) {
	unsigned char uc_val = c_orig_val;
	printf("Assign %d to unsigned char: %hhu\n", c_orig_val, uc_val);
	uc_val>>=1;
	printf("Right shift it by 1: %hhu\n", uc_val);
	// right shifting by CHAR_BIT-2 results in 'uc_val' having only one bit set. The bit set is the LSB.
	uc_val>>=CHAR_BIT-2;
	printf("Then right shift it by CHAR_BIT-2: %hhu\n", uc_val);
	uc_val>>=1; // This should discard the only set bit (1) left
	printf("Then right shift it by 1: %hhu\n", uc_val);
	uc_val>>=1; // See what happens when we shift once more
	printf("Finally right shift it by 1: %hhu\n", uc_val);
	printf("\n");
}

void signed_left_shift_test(char c_orig_val) {
	char c_val = c_orig_val;
	printf("Assign %d to char: %d\n", c_orig_val, c_val);
	c_val<<=2;
	printf("Left shift it by 2: %d\n", c_val);
	// shifting by CHAR_BIT-3 results in 'c_val' having only one bit set. The bit set is the MSB (sign bit).
	c_val<<=CHAR_BIT-3;
	printf("Then left shift it by CHAR_BIT-3: %d\n", c_val);
	c_val<<=1; // This should discard the only set bit (1) left
	printf("Then left shift it by 1: %d\n", c_val);
	c_val<<=1; // See what happens when we shift once more
	printf("Finally left shift it by 1: %d\n", c_val);
	printf("\n");
}

void signed_right_shift_test(char c_orig_val) {
	char c_val = c_orig_val;
	printf("Assign %d to char: %d\n", c_orig_val, c_val);
	c_val>>=2;
	printf("Right shift it by 2: %d\n", c_val);
	// right shifting by CHAR_BIT-3 results in 'c_val' having all bits set (still!!).
	c_val>>=CHAR_BIT-3;
	printf("Then right shift it by CHAR_BIT-3: %d\n", c_val);
	c_val>>=1; // This still results in 'c_val' having all bits set
	printf("Then right shift it by 1: %d\n", c_val);
	c_val>>=1; // See what happens when we shift once more
	printf("Finally right shift it by 1: %d\n", c_val);
	printf("\n");
}

int main(void) {
	unsigned_left_shift_test(-1);
	unsigned_right_shift_test(-1);

	unsigned_left_shift_test(CHAR_MAX);
	unsigned_right_shift_test(CHAR_MAX);
	printf("\n");


	signed_left_shift_test(-1);
	signed_right_shift_test(-1);

	signed_left_shift_test(CHAR_MAX);
	signed_right_shift_test(CHAR_MAX);
	printf("\n");
}
