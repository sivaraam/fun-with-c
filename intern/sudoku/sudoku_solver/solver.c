/**** DEV ****/
#define KS_SUDOKU_DEBUG
//#define KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES
//#define KS_SUDOKU_DEBUG_HIDDEN_SINGLE_SEARCH
/**** DEV *****/

#ifdef KS_SUDOKU_DEBUG
#include <stdio.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "sudoku_solver.h"
#include "common.h"
#include "naked_single_queue.h"
#include "solver_helpers.h"

/**
  * Identify the "hidden singles" and solve (fill in) them.
  * This might help in identifying other possibilities such as "naked singles",
  * "naked doubles" or even other "hidden singles".
  *
  * This is done by identifying values which occur only once in a 'row' ('col' or 'square')
  * and filling that cell with that value and correspindingly updating the possibilities.
  */
bool solve_hidden_singles(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	bool found_hidden_single = false;
	for (unsigned val=MIN_VALUE; val<=MAX_VALUE; val++)
	{

		// search for hidden singles in rows

#ifdef KS_SUDOKU_DEBUG_HIDDEN_SINGLE_SEARCH
		printf("solve_hidden_singles: searching for hidden singles in rows for val: %u\n", val);
#endif

		for (size_t row=0; row<TABLE_ORDER_MAX; row++)
		{
			found_hidden_single |= solve_hidden_singles_helper(sudoku_table, possible_values,
									   row, row,
									   0, TABLE_ORDER_MAX-1,
									   val);
		}

		// search for hidden singles in cols

#ifdef KS_SUDOKU_DEBUG_HIDDEN_SINGLE_SEARCH
		printf("solve_hidden_singles: searching for hidden singles in cols for val: %u\n", val);
#endif

		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			found_hidden_single |= solve_hidden_singles_helper(sudoku_table, possible_values,
									   0, TABLE_ORDER_MAX-1,
									   col, col,
									   val);
		}

		// search for hidden singles in squares

#ifdef KS_SUDOKU_DEBUG_HIDDEN_SINGLE_SEARCH
		printf("solve_hidden_singles: searching for hidden singles in squares for val: %u\n", val);
#endif

		for (size_t row=0; row<TABLE_ORDER_MAX; row+=SQUARE_DIMENSION)
		{
			for (size_t col=0; col<TABLE_ORDER_MAX; col+=SQUARE_DIMENSION)
			{
				found_hidden_single |= solve_hidden_singles_helper(sudoku_table, possible_values,
										   row, row+SQUARE_DIMENSION-1,
										   col, col+SQUARE_DIMENSION-1,
										   val);
			}
		}
	}

#ifdef KS_SUDOKU_DEBUG
	printf("\n");
#endif

	return found_hidden_single;
}

/**
  * Solve (fill in) the "naked single" possibilities in the sudoku table.
  * The 'possible_entries' table should be initialized for the given 'sudoku_table'.
  * The moves are obtained from the tail-queue whose head is 'naked_singles_head'.
  */
void solve_naked_singles(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	while (is_naked_single_available())
	{
		struct naked_single *curr = get_first_naked_single();
		remove_first_naked_single();

		unsigned naked_single = find_naked_single(possible_values, curr->row, curr->col);
		sudoku_table[curr->row][curr->col] = naked_single;
		possible_values[curr->row][curr->col].possibilities--; // not of much use as it zeros the result (possibly helpful for debugging)

#ifdef KS_SUDOKU_DEBUG
		printf("solve_naked_singles: only possibility %u for row: %zu, col: %zu\n",
			naked_single, curr->row, curr->col);
#endif

		update_possibilities(sudoku_table, possible_values, curr->row, curr->col, naked_single);

#ifdef KS_SUDOKU_DEBUG
		printf("solve_naked_singles: Naked single possibilities:\n");
		print_naked_singles();
		printf("\n");
#endif

		free(curr);
	}
}

void solve(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
	   struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
    bool found_hidden_single = false;

#ifdef KS_SUDOKU_DEBUG
    unsigned round = 1;
#endif

    do
    {
	solve_naked_singles(sudoku_table, possible_values);

#ifdef KS_SUDOKU_DEBUG
	printf("solve: Possibility vectors after trying to solve 'naked singles' in round: %u:\n",
		round);
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			if (sudoku_table[row][col] == 0)
			{
				printf("solve_sudoku: %u possible values for row: %zu, col: %zu\n",
					possible_values[row][col].possibilities,
					row, col);
				for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
				{
					if (possible_values[row][col].possible[value] == true)
					{
						printf("%zu\t", value);
					}
				}
				printf("\n\n");
			}
		}
	}
#endif

	found_hidden_single = solve_hidden_singles(sudoku_table, possible_values);

#ifdef KS_SUDOKU_DEBUG
	if (found_hidden_single)
	{
		printf("solve: Possibility vectors after trying to solve 'hidden singles' in round: %u:\n",
			round);

		for (size_t row=0; row<TABLE_ORDER_MAX; row++)
		{
			for (size_t col=0; col<TABLE_ORDER_MAX; col++)
			{
				if (sudoku_table[row][col] == 0)
				{
					printf("solve_sudoku: %u possible values for row: %zu, col: %zu\n",
						possible_values[row][col].possibilities,
						row, col);
					for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
					{
						if (possible_values[row][col].possible[value] == true)
						{
							printf("%zu\t", value);
						}
					}
					printf("\n\n");
				}
			}
		}
	}
	else
	{
		printf("solve: No hidden single found in round: %u.\n", round);
	}

	round++;
#endif

    } while (found_hidden_single == true);

}

void solve_sudoku(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	// the lookup table used to identify the possibilities of different cells
	struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX];

	initialise_naked_single_queue();

#ifdef KS_SUDOKU_DEBUG
	printf("\n");
	printf("solve_sudoku: 'sudoku_table' obtained as input\n");
	print_table(sudoku_table);

	printf("solve_sudoku: Possibility vectors:\n");
#endif

	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			if (sudoku_table[row][col] == 0)
			{
				// initialise the possible values
				initialise_possible_values(sudoku_table, possible_values, row, col);

#ifdef KS_SUDOKU_DEBUG
				printf("solve_sudoku: %u possible values for row: %zu, col: %zu\n", possible_values[row][col].possibilities, row, col);
				for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
				{
					if (possible_values[row][col].possible[value] == true)
					{
						printf("%zu\t", value);
					}
				}
				printf("\n\n");
#endif

				// This could also be done in 'initialise_possible_values_helper'.
				// But doing this here saves us some unwanted checking.
				if (possible_values[row][col].possibilities == 1)
				{
					insert_naked_single(row, col);
				}
			}
		}
	}

#ifdef KS_SUDOKU_DEBUG
	printf("solve_sudoku: Naked single possibilities:\n");
	printf("\n");
#endif

	solve(sudoku_table, possible_values);
}
