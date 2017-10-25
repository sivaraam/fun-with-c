#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	char str[50];
	strcpy(str, "hello is hello");

	char *found = strstr(str, "hello");
	strncpy(found, "god", strlen("god"));

	printf("%s\n", str);
}
