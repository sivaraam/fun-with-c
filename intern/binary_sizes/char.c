#include <stdlib.h>

char arr_global[200]={99};
static char static_arr_global[200]={100};
extern char external_array[300];
char aa=101;

int main(void) {
	char a, b, c;
	char arr[100]={100};
	char *arr_dynamic = malloc(sizeof(char)*100);
	static char x;
	static const char z;
}
