#include <stdio.h>

union {
	int four_char;
	char first_char;
	char second_char;
	char third_char;
} int_char;

union {
	int four_char;
	char let_char[3];
} int_carray;

union {
	int four_char;
	struct {
		char first_char;
		char second_char;
		char third_char;
	}st;
} union_struct;

union {
	int four_char;
	struct {
		char first_char:4;
		char second_char:4;
		char third_char:4;
	}st;
} union_struct_bitfields;

union {
	char ch_val;
	int i_val;
}  char_int = {-1};

int main(void)
{
	int_char.four_char=0x41424344;
	printf("%c %c %c \n", int_char.first_char, int_char.second_char, int_char.third_char);

	int_carray.four_char=0x41424344;
	printf("%c %c %c \n", int_carray.let_char[0], int_carray.let_char[1], int_carray.let_char[2]);

	union_struct.four_char=0x41424344;
	printf("%c %c %c \n", union_struct.st.first_char, union_struct.st.second_char, union_struct.st.third_char);

	union_struct_bitfields.four_char=0x41424344;
	printf("%x %x %x \n", union_struct_bitfields.st.first_char, union_struct_bitfields.st.second_char, union_struct_bitfields.st.third_char);

//	char_int.ch_val = 1024;
	printf("char: %d\t int: %d\n", char_int.ch_val, char_int.i_val);
}
