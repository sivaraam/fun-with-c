#include <stdio.h>
#include "common.h"
#include "sudoku_solver.h"

void print_welcome_message(void)
{
	printf("\n\tWelcome to the Sudoku solver!\n");
	printf("\nIt tries to solve sudoku by using the following techniques:\n\n");
	printf("\t1. Naked single detection\n\t2. Hidden single detection\n\t3. Naked double detection\n\n");
	printf("Enter a VALID sudoku table:\n");
	printf("A VALID sudoku table satisfies the following criteria\n");
	printf("\t- It is a 9*9 table\n");
	printf("\t- A cell that is yet to be filled is represented by 0\n");
	printf("\t- A cell only has valid entries 0-9, where 0 represents an empty cell\n");
}

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

void print_solution(unsigned int table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	printf("\nSolved table:\n");
	print_table(table);
	printf("Note: Solutions might be partial if the table cannot be solved using techniques mentioned before.\n");
}

int main(void)
{
	unsigned table[TABLE_ORDER_MAX][TABLE_ORDER_MAX] = {0};
	print_welcome_message();
	get_table(table);
	solve_sudoku(table);
	print_solution(table);
}
