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

/**
 * Returns zero if the read was successful else returns a non-zer valud in case of an error.
 */
int get_table(unsigned int table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	printf ("Only enter valid inputs (0-9). Invalid inputs are discarded.\n");
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			printf ("Enter the value for cell at row: %zu, col: %zu: ", row+1, col+1);
			int input = EOF;
			do {
				input = getchar();
				if (input == EOF)
				{
					printf ("Could not sccessfully read the input!\n");
					return 1;
				}
				if (input >= '0' && input <= '9')
				{
					table[row][col] = input - '0';
					break;
				}
			} while (1);
		}
	}

	return 0;
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
	if (get_table(table))
	{
		return 1;
	}
	solve_sudoku(table);
	print_solution(table);
}
