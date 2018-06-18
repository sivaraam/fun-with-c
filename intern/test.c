#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	printf("size_t: %zu short: %zu char: %zu\n", sizeof(size_t), sizeof(short), sizeof(char));

	struct st
	{
		char *name;
	};

	struct st obj;
	obj.name = "Hello";
	obj.name = malloc(sizeof(char)*20);
	sscanf("Hello", "%s", obj.name);

	printf("%s", obj.name);
//	*name = "Hello"

	int a;
//	scanf("%d", a); SEG fault

	unsigned short val = 40000;
	signed short s_val = val;
	signed short sc_val = (signed short)val;

	printf("unsigned short: %hu\n", val);
	printf("signed short: %hd\n", s_val);
	printf("signed short (cast): %hd\n", sc_val);

	// comparison between signed and unsigned
	unsigned u_val = INT_MAX;
	u_val++;
	if (u_val > INT_MAX)
	{
		printf("%u doesn't fit in an int\n", u_val);
	}

	// test freeing a const pointer
	char *const char_ptr = malloc(sizeof(1));
	*char_ptr = '8';
	free(char_ptr);
}
