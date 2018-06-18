#include <stdio.h>
#include <string.h>
#include <limits.h>

char *char_to_binary(char ch) {
	unsigned char single_bit_set = 1<<(CHAR_BIT-1);
	size_t bit = 0;
	unsigned char binary_ch[CHAR_BIT+1] = {0};
	while (bit<CHAR_BIT) {
		binary_ch[bit++] = (ch & single_bit_set) ? '1' : '0';
		single_bit_set>>=1;
	}
	binary_ch[bit] = '\0';
	return strndup(binary_ch, CHAR_BIT+1);
}

int main(void) {
	char choice = 'y';
	do {
		char cuser_input='A';
		printf("Enter a non-whitespsace character: ");
		scanf(" %1c", &cuser_input);
		printf("ASCII value: %d\n", cuser_input);
		printf("Binary representation: %s\n\n", char_to_binary(cuser_input));
		do {
			printf("Do you wish to continue (y/n): ");
			scanf(" %1c", &choice);
		} while ((choice != 'y' && choice != 'Y') && (choice != 'n' && choice !='N'));
	} while (choice == 'y' | choice == 'Y');
}

