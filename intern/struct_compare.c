#include <stdio.h>

int main(void)
{
	struct
	{
		ssize_t val_1, val_2;
	} test_1 = {-1, -1}, test_2 = {-1, -1}, test_3 = {0, 5};

	if (test_1 == test_2)
	{
		printf("'test_1' and 'test_2' point to same object/have same values.\n");
	}

	if (test_1 == test_2)
	{
		printf("'test_1' and 'test_2' have same value!\n");
	}
	else
	{
		printf("'test_1' and 'test_2' point to different objects!\n");
	}

	if (test_1 == test_3)
	{
		printf("'test_1' and 'test_3' point to different objects/have different values.\n");
	}
}
