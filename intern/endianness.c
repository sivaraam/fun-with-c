#include <stdio.h>
#include <string.h>

int main(void) {
	short sitest = 0x1011;
	char *ctest = (char*) &sitest;
	const static char little_endian_byte = (char) 0x11;

	/*
		Is it a correct check for endianness if,
			char  - 1 byte

		char ctest = (char) sitest;
		printf("%hx", ctest)
	*/

	if (*ctest == little_endian_byte)
		printf("You have a little endian architecture\n");
	else
		printf("You have a big endian architecture\n");
}
