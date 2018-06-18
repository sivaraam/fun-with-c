#include <stdlib.h>

int arr_global[200];//={999};
static int static_arr_global[200];//={100};
//extern int external_array[300];
//int aa;//=1010;

int main(void) {
//	int a, b, c;
//	a=a+123123;
	//int arr[100];//={100};
	int *arr_dynamic = malloc(sizeof(int)*100);
//	static int x=10;
	static const int z;//=2948;
}
