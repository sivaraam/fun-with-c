#include <stdio.h>

int main(void)
{
	printf("_Alignof(char): %zu\n", _Alignof(char));
	printf("_Alignof(_Bool): %zu\n", _Alignof(_Bool));
	printf("_Alignof(short): %zu\n", _Alignof(short));
	printf("_Alignof(int): %zu\n", _Alignof(int));
	printf("_Alignof(long): %zu\n", _Alignof(long));
	printf("sizeof(long): %zu\t sizeof(long long): %zu\n", sizeof(long), sizeof(long long));
	printf("_Alignof(long long): %zu\n", _Alignof(long long));
	printf("_Alignof(float): %zu\n", _Alignof(float));
	printf("_Alignof(double): %zu\n", _Alignof(double));
//	printf("_Alignof(struct): %zu\n", _Alignof(struct));
}
