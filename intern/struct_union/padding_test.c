#include <stdio.h>

int main(void)
{
	struct one_char
	{
		char a;
	};

	printf("struct one_char:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct one_char),  _Alignof(struct one_char));

	struct chars
	{
		char a;
		char b;
		char c;
	};

	printf("struct chars:\t\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct chars),  _Alignof(struct chars));

	struct pointers
	{
		char *c_ptr;
		int *i_ptr;
	};

	printf("struct pointers:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct pointers), _Alignof(struct pointers));

	struct one_int
	{
		int a;
	};

	printf("struct one_int:\t\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct one_int),  _Alignof(struct one_int));


	struct ints
	{
		int a, b, c;

	};

	printf("struct ints:\t\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct ints),  _Alignof(struct ints));

	struct int_array
	{
		int arr[3];
	};

	printf("struct int_array:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct int_array), _Alignof(struct int_array));

	struct short_chars
	{
		char a;
		short s_int;
		char b;
	};

	printf("struct short_chars:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct short_chars), _Alignof(struct short_chars));

	struct ints_chars
	{
		int a;
		char b;
		int d;
		char e;
	};

	struct ints_double
	{
		int a;
		double d_bits;
	};

	printf("struct ints_double:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct ints_double),  _Alignof(struct ints_double));

	struct str_1
	{
		char ab;
	};

	struct str
	{
		char a;
		int d;
		struct str_1 c;
		double e;
	};

	printf("nested structure:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct str), _Alignof(struct str));

	struct bitfields
	{
		int seven_bits : 7;
		char five_bits : 5;
		int i;
	};

	printf("struct bitfields:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct bitfields), _Alignof(struct bitfields));

	struct long_double
	{
		int a;
		long long l;
		char c;
		double d;
	};

	printf("struct long_double:\t sizeof(): %zu\t _Alignof(): %zu\n", sizeof(struct long_double), _Alignof(struct long_double));

	struct {
		char c[2];
		int val;
	} pad_test;

	pad_test.c[0] = 'H';
	pad_test.c[1] = 'e';
	pad_test.val=0xABCDEF;
	printf("Structure character contents: %d %d %d %d %x\n", pad_test.c[0], pad_test.c[1], pad_test.c[2], pad_test.c[3], pad_test.c[4]);
}
