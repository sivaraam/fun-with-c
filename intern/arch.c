#include <stdio.h>
#include <stdbool.h>

bool is32bit() {
	if(((size_t)-1) == 0xffffffffu)
		return true;
	else
		return false;
}

int main(void) {
	if (is32bit())
		printf("You have a 32bit architecture\n");
	else
		printf("You have a 64bit architecture\n");
}
