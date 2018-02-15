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

/**
  * Type to hold the possible values for a cell (identified by row and column).
  */
struct possible_entries
{
	// an index is set to 'true' if the corresponding value is possible.
	// Indexed from 1 to MAX_VALUE.
	bool possible[MAX_VALUE+1];

	// the number of possible values for a cell
	unsigned possibilities;
};

/**
  * Returns the "naked single" value for the given cell.
  */
unsigned find_naked_single(struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				size_t row, size_t col)
{
	for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
	{
		if (possible_values[row][col].possible[value] == true)
		{
			return value;
		}
	}

#ifdef KS_SUDOKU_DEBUG
	fprintf(stderr, "find_naked_single: invalid request.\n"
			"row: %zu, col: %zu has %u possibilities!\n", row, col, possible_values[row][col].possibilities);
	fprintf(stderr, "find_naked_single: possibility vector: \n");
	for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
		fprintf(stderr, "%zu: %d\t", value, possible_values[row][col].possible[value]);
//	print_table(sudoku_table);
	exit(EXIT_FAILURE);
#endif

	return 0; // for safety
}

/**
  * Used to avoid redundancy in the 'update_possibility' function.
  */
void update_possibilities_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 size_t row, size_t col, unsigned val,
				 size_t search_row_start, size_t search_row_end,
				 size_t search_col_start, size_t search_col_end)
{
	// search the remaining cells of the square for values which are not possible
	for (size_t search_row=search_row_start; search_row<=search_row_end; search_row++)
	{
		for (size_t search_col=search_col_start; search_col<=search_col_end; search_col++)
		{
			if (
			    // During a row update, ignore the col that caused the update
			    (search_row_start == search_row_end && search_col == col) ||

			    // During a col update, ignore the row that caused the update
			    (search_col_start == search_col_end && search_row == row) ||

			    // During a square update, ignore the row and col which have already been updated
			    ((search_row_start != search_row_end && search_col_start != search_col_end) &&
			     (search_row == row || search_col == col))
			)
			{

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES
				printf("update_possibilities_helper: skipping row: %zu, col: %zu\n"
					"search_row_start: %zu\t search_row_end: %zu\n"
					"search_col_start: %zu\t search_col_end: %zu\n",
					search_row, search_col,
					search_row_start, search_row_end,
					search_col_start, search_col_end);
#endif

				continue;
			}

			if (sudoku_table[search_row][search_col] == 0 &&
			    possible_values[search_row][search_col].possible[val] != false)
			{
				possible_values[search_row][search_col].possible[val] = false;
				possible_values[search_row][search_col].possibilities--;
				if (possible_values[search_row][search_col].possibilities == 1)
				{

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES
					printf("update_possibilities_helper: only_possibility: %u for row: %zu, col: %zu\n",
						find_naked_single(possible_values, search_row, search_col),
						search_row, search_col);
#endif

					insert_naked_single(search_row, search_col);
				}

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES
				else if (possible_values[search_row][search_col].possibilities == 0)
				{
					fprintf(stderr, "update_possibilities_helper: incorrect move.\n");
					fprintf(stderr, "update_possibilities_helper: updating row: \
 							%zu, col: %zu with %u\n", row, col, val);
					fprintf(stderr, "update_possibilities_helper: left \
							row: %zu, col: %zu with no possibilities\n",
							search_row, search_col);
					print_table(sudoku_table);
					exit(EXIT_FAILURE);
				}
				else
				{
					printf("update_possibilities_helper: %u possibilities for row: %zu, col: %zu\n",
						possible_values[search_row][search_col].possibilities,
						search_row, search_col);
				}
#endif

			}
		}
	}

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES
	printf("\n");
#endif
}

/*
 * Update the possibilities of cells as a consequence of assignment of 'val' to the given cell.
 *
 *  sudoku_table - the table containing the sudoku board
 *  possible_values - lookup table for possible values of different cells in the
 *                    sudoku board.
 *  (row, col) - the cell due to which the update is being performed
 *  val - the value being inserted into (row, col)
 *  (search_row_start, search_row_end) - the range of rows which must be updated
 *  (search_col_start, search_col_end) - the range of colums which must be updated
 */
void update_possibilities(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			  struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			  size_t row, size_t col, unsigned val)
{
	// update the cells in the same row
	update_possibilities_helper(sudoku_table, possible_values, row, col, val,
				    row, row,
				    0, TABLE_ORDER_MAX-1);

	// update the cells in the same column
	update_possibilities_helper(sudoku_table, possible_values, row, col, val,
				    0, TABLE_ORDER_MAX-1,
				    col, col);

	// find corners of square
	const size_t top_left_row = (row/3)*3;
	const size_t top_left_col = (col/3)*3;

	// update the remaining cell in the square
	update_possibilities_helper(sudoku_table, possible_values, row, col, val,
				    top_left_row, top_left_row+SQUARE_DIMENSION-1,
				    top_left_col, top_left_col+SQUARE_DIMENSION-1);
}

/**
  * Helper function to avoid redundancy in the 'solve_naked_singles' function.
  *
  * sudoku_table - the table containing the sudoku board
  *  possible_values - lookup table for possible values of different cells in the
  *                    sudoku board.
  *  (search_row_start, search_row_end) - the range of rows which must be searched
                                          for a "hidden single"
  *  (search_col_start, search_col_end) - the range of colums which must be searched
                                          for a "hidden single"
  * val - the "hidden single" value which is being searched for
  *
  * Returns true if the 'val' is found as a hidden single in the search space.
  */
bool solve_hidden_singles_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 size_t search_row_start, size_t search_row_end,
				 size_t search_col_start, size_t search_col_end,
				 unsigned val)
{
	bool found_hidden_single = false;
	unsigned occurrence = 0;
	size_t possible_row = 0, possible_col = 0;
	for (size_t search_row=search_row_start; search_row<=search_row_end; search_row++)
	{
		for (size_t search_col=search_col_start; search_col<=search_col_end; search_col++)
		{
			if (sudoku_table[search_row][search_col] == 0)
			{
				if (possible_values[search_row][search_col].possible[val] == true)
				{
					occurrence++;
					possible_row = search_row;
					possible_col = search_col;
				}
			}
		}
	}

	if (occurrence == 1u)
	{

#ifdef KS_SUDOKU_DEBUG
		printf("solve_hidden_singles_helper: found hidden single %u for row: %zu, col: %zu\n",
			val, possible_row, possible_col);
#endif

		sudoku_table[possible_row][possible_col] = val;
		update_possibilities(sudoku_table, possible_values, possible_row, possible_col, val);
		found_hidden_single = true;
	}

	return found_hidden_single;
}

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

/**
  * Used to avoid redundancy in the 'initialise_possible_values' function.
  */
void initialise_possible_values_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				       struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				       size_t row, size_t col,
				       size_t search_row_start, size_t search_row_end,
				       size_t search_col_start, size_t search_col_end)
{
	for (size_t search_row=search_row_start; search_row<=search_row_end; search_row++)
	{
		for (size_t search_col=search_col_start; search_col<=search_col_end; search_col++)
		{
			if (
				// During a row initialization, ignore the col that caused the initialisation
				(search_row_start == search_row_end && search_col == col) ||

				// During a col update, ignore the row that caused the initialisation
				(search_col_start == search_col_end && search_row == row) ||

				// During a square update, ignore the row and col whose
				// possibilities have already been initialised
				((search_row_start != search_row_end && search_col_start != search_col_end) &&
				 (search_row == row || search_col == col))
			)
			{
				continue;
			}

			const unsigned val = sudoku_table[search_row][search_col];
			if (val != 0)
			{
				if (possible_values[row][col].possible[val] != false) // avoid multiple counting
				{
					possible_values[row][col].possible[val] = false;
					possible_values[row][col].possibilities--;
				}
			}
		}
	}
}

/**
  * Initialise 'possible_entries' with values that are possible for a given sudoku cell.
  * Obviously the cell is expected not to be an already filled one.
  *
  * Initialization is done by identifying values that are not possible for the given square
  * by linearly searching the corresponding row, column and the square. The 'possible_entries'
  * is updated accordingly.
  */
void initialise_possible_values(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				size_t row, size_t col)
{

#ifdef KS_SUDOKU_DEBUG
	if (sudoku_table[row][col] != 0)
	{
		fprintf(stderr, "initialise_possible_values: Trying to initialise an already filled cell\n");
		exit(EXIT_FAILURE);
	}
#endif

	// initially initialise all possibilities to 'true'
	for (size_t val=MIN_VALUE; val<=MAX_VALUE; val++)
	{
		possible_values[row][col].possible[val] = true;
	}
	possible_values[row][col].possibilities = NUMBER_OF_VALUES;

	// initialise possible_values of cell by searching values in the same row
	initialise_possible_values_helper(sudoku_table, possible_values,
					  row, col,
					  row, row,
					  0, TABLE_ORDER_MAX-1);

	// intialise possible_values of cell by searching values in the same column
	initialise_possible_values_helper(sudoku_table, possible_values,
					  row, col,
					  0, TABLE_ORDER_MAX-1,
					  col, col);

	// find corners of square
	const size_t top_left_row = (row/3)*3;
	const size_t top_left_col = (col/3)*3;

	// intialise possible_values of cell by searching values in the same square
	initialise_possible_values_helper(sudoku_table, possible_values,
					  row, col,
					  top_left_row, top_left_row+SQUARE_DIMENSION-1,
					  top_left_col, top_left_col+SQUARE_DIMENSION-1);
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
