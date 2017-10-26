#include <stddef.h>
#include <string.h>
/*
 * Adds the given character before the end of 'str'.
 * As is usual, assumes string is '\0' (NUL) terminated.
 */
void append_ch(char *str, char ch) {
	size_t len = strlen(str);
	*(str+len) = ch;
	*(str+len+1) = '\0';
}

