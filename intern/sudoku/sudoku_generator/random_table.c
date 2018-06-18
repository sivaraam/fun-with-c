#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROW_MAX 9
#define COL_MAX 9
#define MAX_VAL 9

void print_new_permutation()
{
	bool value_seen[MAX_VAL+1] = {0};
	int permutation[MAX_VAL+1] = {0};
	size_t permutation_count = 0;

	while (permutation_count != MAX_VAL)
	{
		int rand_val = rand()%(MAX_VAL+1);
		if (rand_val != 0 && value_seen[rand_val] != true)
		{
			permutation[permutation_count] = rand_val;
			value_seen[rand_val] = true;
			permutation_count++;
		}
	}
	printf("Permutation of 1-%d: \n", MAX_VAL);
	for (size_t i=0; i<MAX_VAL; i++)
	{
		printf("%d ", permutation[i]);
	}

	printf("\n");
}

int main(void)
{
	for (size_t row=0; row<ROW_MAX; row++)
	{
		for (size_t col=0; col<COL_MAX; col++)
		{
			int rand_val = 0;
			while (rand_val == 0)
				rand_val = rand()%(MAX_VAL+1);
			printf("%d ", rand_val);
		}
		printf("\n");
	}

	printf("Generating permutation of 1-%d ...\n", MAX_VAL);
	print_new_permutation();
	print_new_permutation();
	print_new_permutation();

}
