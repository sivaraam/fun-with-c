#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	void *ptr = malloc(sizeof(int));
	char *c_ptr;
	// sizeof doesn't evaluate it's arguments
	printf("void*: %p\t sizeof(void*): %zu\t sizeof(*(void*)): %zu\n", ptr, sizeof(ptr), sizeof(*ptr));

	// printf("Access void pointer: %d\n", *ptr); error

	/*
	 * This is fine in GCC!! It allows this as a compiler extension.
	 * As a consequence sizeof(void) is alos allowed.
	 */
	ptr++; 

	printf("void*: %p\t sizeof(void*): %zu\t sizeof(*(void*)): %zu\n", ptr, sizeof(ptr), sizeof(*ptr));
	printf("char*: %p\t sizeof(char*): %zu\t sizeof(*(char*)): %zu\n", c_ptr, sizeof(c_ptr), sizeof(*c_ptr));

	char *base_plus_one_ptr = calloc(2, sizeof(char));
	base_plus_one_ptr++;
//	free(base_plus_one_ptr);  invalid free
	base_plus_one_ptr--;
	free(base_plus_one_ptr);

	char *(ch[3]);  // array of 3 pointers to characters

	char *ch_p = malloc(sizeof(char));
	char *temp = realloc(ch_p, 2*sizeof(char));
	if (temp == NULL)
	{
		printf("realloc failed!\n");
	}

	char *tmp = realloc(ch_p, 0);
	if (tmp == NULL)
	{
		printf("realloc returned NULL.\n");
	}
}
