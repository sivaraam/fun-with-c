#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *ch = malloc(1);
	FILE *grep_file = fopen("grep-man", "r+");

	printf("%c %c", fgetc(grep_file), fgetc(grep_file)); // good example of where the order of evaluation cannot be specified correctly.
	fgetc(grep_file);

	fprintf(grep_file, "%s", "Hello world");
//	fprintf(grep_file, "%s", ""); does nothing

	printf("%c\n", fgetc(grep_file));
}
