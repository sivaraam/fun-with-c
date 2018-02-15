#include "common.h"

#ifdef KS_SUDOKU_DEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

#include "sudoku_solver.h"
#include "naked_single_queue.h"
#include "solver_helpers.h"

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
static void update_possibilities_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
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
  * Used to avoid redundancy in the 'initialise_possible_values' function.
  */
static void initialise_possible_values_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
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

