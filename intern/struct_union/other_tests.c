#include <stdio.h>
#include <stdint.h>

const struct value
{
	int val;
} const_val;

void foo()
{
	static struct value_1
	{
		int val;
	} static_val;
	static_val.val++;

	struct value_1 val1_obj = {200};
	val1_obj.val++;

	printf("static_val: %d\t val1_obj.val: %d\n", static_val.val, val1_obj.val);
}

int main(void)

{
	printf("const_val: %d\n", const_val.val);

	typedef struct value value;
//	typedef struct value; does not create type alias
	value val_obj;

	val_obj.val = 1000;
//	const_val.val = 1000;	error: assignment of member ‘val’ in read-only object

	printf("val_obj: %d\n", val_obj.val);

	foo();
	foo();
	foo();

	// Bitfield manipulation
	typedef struct bitfield
	{
		int int_bits:4;
		char ch_bits:5;
	} bitfield;

	bitfield bits = {.ch_bits = 31, .int_bits=16}; // designated initialization
//	bits.ch_bits = 31, bits.int_bits=16;
	printf("%d %d\n", bits.int_bits, bits.ch_bits);

	bits.ch_bits++;
	printf("%d\n", bits.ch_bits);

	struct value
	{
		int bit1:1;
		int bit3:4;
		int bit4:4;
	} bit={1, 2, 13};
	printf("%d, %d, %d\n", bit.bit1, bit.bit3, bit.bit4);

	// Explore enumeration
	enum boolean {true, false};
	enum boolean b = true;
	b = 5;
	b++;
	printf("sizeof(enum boolean): %zu\n", sizeof(enum boolean));
	printf("b: %d\n", b);
//	true++;

	// Explore unions
	union
	{
		float x;
		uint8_t uint;
	} sensor_data;

	sensor_data.x = 19.34;
	printf("float in a union: %f\n", sensor_data.x);
	printf("uint8_t counter-part of float in a union: %d\n", sensor_data.uint);

}
