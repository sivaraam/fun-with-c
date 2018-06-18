#include <stdio.h>

int foo()
{
	static int a = 16;
	return a--;
}

int main(void)
{

	printf("Question 8:\n");
	int a = 0, b = 1, c = 2;
	*(!(a+2==1) ? &b : &a) = a++ ? c : b;
	printf(a ? "%d %d %d" : "Compiler error", a, b, c);
	printf("\n");

	printf("Question 9:\n");
	union test
	{
		int x;
		char arr[4];
		int y;
	} t;

	t.x = 0;
	t.arr[1] = 'G';  // index STARTS from 0 !!!
	printf("%s", t.arr);
	printf("\n");

	printf("Quesiton 5:\n");
	for (foo(); foo(); foo())
		printf("%d ", foo());
	printf("\n");

	printf("Question 6:\n");
	char arr[] = {"ABCD"};
	unsigned int *ptr = arr;
	*ptr = *ptr >> 16 | *ptr << 16;
	printf("%s", ptr);
	printf("\n");

	return 0;
}
