#include <stdio.h>
#include <stdlib.h>
#define NUM 10

int main(void)
{
	int **double_ints = calloc(NUM, sizeof(int*));

	printf("Storing values:\n");
	for (unsigned int i=0; i<NUM; i++)
	{
		int *temp = malloc(sizeof(int));
		*temp = rand()%20;
		printf("%d\t", *temp);
		*(double_ints+i) = temp;
	}
	printf("\n");

	printf("Values stored:\n");
	for (unsigned int j=0; j<NUM; j++)
	{
		printf("%d\t", *(*(double_ints+j)));
		free(*(double_ints+j));
	}
	printf("\n");

	// try freeing the pointers one by one
	// regardless of obtaining them together
	for (unsigned int k=0; k<NUM; k++)
	{
		printf("Trying to free %p\n", double_ints+k);
		free(double_ints+k);
	}
}
