#include <stdio.h>
#include "sudoku_solver.h"

void print_welcome_message(void) {
	printf("\n\tWelcome to the Sudoku solver!\n");
	printf("\nIt tries to solve sudoku by identifying and filling forced cells.\n\n");
	printf("Enter a VALID sudoku table:\n");
	printf("A VALID sudoku table satisfies the following criteria\n");
	printf("\t- It is a 9*9 table\n");
	printf("\t- A cell that is yet to be filled is represented by 0\n");
	printf("\t- A cell only has valid entries [0, 9]\n");
}

void get_table(unsigned int table[TABLE_ORDER_MAX][TABLE_ORDER_MAX]) {
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			scanf("%u", &table[row][col]);
		}
	}
}


/*void print_table(unsigned **table) {
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			printf("%d", table[row][col]);
		}
	}
}*/

int main(void) {
	unsigned table[TABLE_ORDER_MAX][TABLE_ORDER_MAX] = {0};
	print_welcome_message();
	get_table(table);
	solve_sudoku(table);
}
