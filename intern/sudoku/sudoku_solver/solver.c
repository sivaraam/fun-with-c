#include "common.h"

#ifdef KS_SUDOKU_DEBUG
#include <stdio.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "sudoku_solver.h"
#include "naked_single_queue.h"
#include "solver_helpers.h"

/**
  * Idenitfy the "naked doubles" in a group (row, column or square) and update
  * the corresponding group to remove them as possibilities.
  * This might reveal other possibilities such as "naked singles", "hidden signles"
  * or even other "naked doubles".
  */
static bool solve_naked_doubles(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	bool found_naked_double = false;

	// search for naked doubles in rows

#ifdef KS_SUDOKU_DEBUG_NAKED_DOUBLE_SEARCH
	printf("solve_naked_doubles: searching for naked doubles in rows\n");
#endif

	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		found_naked_double |= solve_naked_doubles_helper(sudoku_table, possible_values,
								row, row,
								0, TABLE_ORDER_MAX-1);
	}

	// search for naked doubles in cols

#ifdef KS_SUDOKU_DEBUG_NAKED_DOUBLE_SEARCH
	printf("solve_naked_doubles: searching for naked doubles in cols\n");
#endif

	for (size_t col=0; col<TABLE_ORDER_MAX; col++)
	{
		found_naked_double |= solve_naked_doubles_helper(sudoku_table, possible_values,
								0, TABLE_ORDER_MAX-1,
								col, col);
	}

#ifdef KS_SUDOKU_DEBUG_NAKED_DOUBLE_SEARCH
	printf("solve_naked_doubles: searching for naked doubles in squares\n");
#endif

	// search for naked doubles in squares

	for (size_t row=0; row<TABLE_ORDER_MAX; row+=SQUARE_DIMENSION)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col+=SQUARE_DIMENSION)
		{
			found_naked_double |= solve_naked_doubles_helper(sudoku_table, possible_values,
									 row, row+SQUARE_DIMENSION-1,
									 col, col+SQUARE_DIMENSION-1);
		}
	}

	return found_naked_double;
}

/**
  * Identify the "hidden singles" and solve (fill in) them.
  * This might help in identifying other possibilities such as "naked singles",
  * "naked doubles" or even other "hidden singles".
  *
  * This is done by identifying values which occur only once in a group (row, column or square)
  * and filling that cell with that value and correspindingly updating the possibilities.
  */
static bool solve_hidden_singles(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
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
static void solve_naked_singles(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	while (is_naked_single_available())
	{
		struct naked_single *curr = get_first_naked_single();
		remove_first_naked_single();

		unsigned naked_single = find_naked_single(possible_values, curr->row, curr->col);
		sudoku_table[curr->row][curr->col] = naked_single;

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

static void solve(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
	   struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
    bool try_next_round = false;
    unsigned round = 1;
    static const unsigned round_limit = 100;

    do
    {
	solve_naked_singles(sudoku_table, possible_values);

#ifdef KS_SUDOKU_DEBUG
	printf("solve: Possibility vectors after trying to solve 'naked singles' in round: %u:\n",
		round);
	print_possibility_vector(sudoku_table, possible_values);
#endif

	bool found_hidden_single = solve_hidden_singles(sudoku_table, possible_values);

#ifdef KS_SUDOKU_DEBUG
	if (found_hidden_single)
	{
		printf("solve: Possibility vectors after trying to solve 'hidden singles' in round: %u:\n",
			round);
		print_possibility_vector(sudoku_table, possible_values);
	}
	else
	{
		printf("solve: No 'hidden single' found in round: %u.\n", round);
	}
#endif

	bool found_naked_double = solve_naked_doubles(sudoku_table, possible_values);

#ifdef KS_SUDOKU_DEBUG
	if (found_naked_double)
	{
		printf("solve: Possibility vectors after trying to solve 'naked doubles' in round: %u:\n", round);
		print_possibility_vector(sudoku_table, possible_values);
	}
	else
	{
		printf("solve: No 'naked double' found in round: %u.\n", round);
	}
#endif

	try_next_round = found_hidden_single | found_naked_double;
	round++;
    } while (try_next_round == true &&
	     (round <= round_limit)); // to avoid infinite looping due to repeated detection of "naked doubles"

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
#endif

	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			if (sudoku_table[row][col] == 0)
			{
				// initialise the possible values
				initialise_possible_values(sudoku_table, possible_values, row, col);

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
	printf("solve_sudoku: Possibility vector after initialization:\n");
	print_possibility_vector(sudoku_table, possible_values);

	printf("solve_sudoku: Naked single possibilities:\n");
	print_naked_singles();
	printf("\n");
#endif

	solve(sudoku_table, possible_values);
}
