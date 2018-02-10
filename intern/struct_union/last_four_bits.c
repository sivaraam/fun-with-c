#include <stdio.h>
#include <limits.h>
#include <string.h>

#define NUM_BITS 4

char *unsigned_to_binary(unsigned val) {
        unsigned char single_bit_set = 1<<(NUM_BITS-1);
        size_t bit = 0;
        unsigned char binary_ch[NUM_BITS+1] = {0};
        while (bit<NUM_BITS) {
                binary_ch[bit++] = (val & single_bit_set) ? '1' : '0';
                single_bit_set>>=1;
        }
        binary_ch[bit] = '\0';
        return strndup(binary_ch, NUM_BITS+1);
}

int main(void)
{
	union
	{
		int val;
		unsigned last_NUM_BITS_bits : NUM_BITS;
	} values[8] = {INT_MIN, -1, 1729, 1996, 1991, 21, 1919, 1947};

	for (size_t i=0; i<8; i++)
		printf("%d\t %s\n", values[i].val, unsigned_to_binary(values[i].last_NUM_BITS_bits));
}
