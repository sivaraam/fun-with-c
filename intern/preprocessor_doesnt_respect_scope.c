#include <stdio.h>

void foo()
{
	#define FOO 100
}

int main(void)
{
	printf("%d", FOO);
}
