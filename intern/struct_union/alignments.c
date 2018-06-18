#include <stdio.h>

int main(void)
{
	printf("sizeof(char: %zu\t\t _Alignof(char): %zu\n", sizeof(char),  _Alignof(char));
	printf("sizeof(_Bool): %zu\t _Alignof(_Bool): %zu\n", sizeof(_Bool), _Alignof(_Bool));
	printf("sizeof(short):%zu\t\t _Alignof(short): %zu\n", sizeof(short), _Alignof(short));
	printf("sizeof(int): %zu\t\t _Alignof(int): %zu\n", sizeof(int), _Alignof(int));
	printf("sizeof(long): %zu\t\t _Alignof(long): %zu\n", sizeof(long), _Alignof(long));
	printf("sizeof(long long): %zu\t _Alignof(long long): %zu\n", sizeof(long long), _Alignof(long long));
	printf("sizeof(float): %zu\t _Alignof(float): %zu\n", sizeof(float), _Alignof(float));
	printf("sizeof(double): %zu\t _Alignof(double): %zu\n", sizeof(double), _Alignof(double));
//	printf("_Alignof(struct): %zu\n", _Alignof(struct));
}
