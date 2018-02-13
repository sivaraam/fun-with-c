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
	scanf("%d", a);
}
