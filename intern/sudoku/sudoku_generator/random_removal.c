#include <stdio.h>
#include <stdlib.h>

#define TABLE_ORDER_MAX 9

void get_table(unsigned int table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
		for (size_t row=0; row<TABLE_ORDER_MAX; row++)
		{
			for (size_t col=0; col<TABLE_ORDER_MAX; col++)
			{
				scanf("%u", &table[row][col]);
			}
		}
}

void print_table(unsigned int table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			printf("%u\t", table[row][col]);
		}
		printf("\n\n");
	}
}


// See what happens if we randomly remove two cells
void randomly_remove(unsigned table[TABLE_ORDER_MAX][TABLE_ORDER_MAX], size_t row, size_t col)
{
	table[row][col] = 0;
	table[row][col+1] = 0;

	// remove cell of symmetric table
	table[TABLE_ORDER_MAX-row-1][TABLE_ORDER_MAX-col-1] = 0;
	table[TABLE_ORDER_MAX-row-1][TABLE_ORDER_MAX-col-2] = 0;
}
int main(void)
{
	unsigned table[TABLE_ORDER_MAX][TABLE_ORDER_MAX] = {0};
	get_table(table);
	randomly_remove(table, 1, 0);
	randomly_remove(table, 1, 6);
	print_table(table);
}
