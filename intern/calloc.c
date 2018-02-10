#include <stdio.h>
//#include <stdlib.h>
#include <stdint.h> // SIZE_MAX
#include <limits.h> // CHAR_MAX
//#include <unistd.h>
//#include <time.h>

int main(void) {
	char *p = NULL;
	size_t alloc_size = SIZE_MAX;
//	struct timespec t = {1000, 100000};
	while (p == NULL) {
		printf("Trying to allocate %zu bytes\n", alloc_size);
		p = calloc(1u, alloc_size);
		if (p == NULL)
			alloc_size /= 2u;
	}
	printf("Successfully allocated %zu bytes\n", alloc_size);
//	nanosleep(&t, &t);
//	printf("errno: %d", errno);					????
	size_t i = 0;
	while (1) {
		*(p+i) = rand() % CHAR_MAX;
		i=(i+1)%alloc_size;
	}
}
