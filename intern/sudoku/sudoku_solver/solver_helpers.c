#include "common.h"

#ifdef KS_SUDOKU_DEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

#include "solver_helpers.h"
#include "naked_single_queue.h"

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
	fprintf(stderr,"\n");
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
  * If the given cells constitute a "naked double" it returns a 'struct naked_double_values' object
  * with values initialized to the corresponding double.
  * Else returns a 'struct naked_double_values' with values set to 0.
  */
static struct naked_double_values same_dual_possibility(struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
							 size_t row_1, size_t col_1,
							 size_t row_2, size_t col_2)
{

	struct naked_double_values doubles = { {0, 0} };
	unsigned val_1 = 0, val_2 = 0;

#ifdef KS_SUDOKU_DEBUG
	if (possible_values[row_1][col_1].possibilities != 2 ||
	    possible_values[row_2][col_2].possibilities != 2)
	{
		fprintf(stderr, "same_dual_possibility: Invalid cells.\n");
		fprintf(stderr, "same_dual_possibility: row_1: %zu, col_1: %zu\n", row_1, col_1);
		fprintf(stderr, "same_dual_possibility: row_2: %zu, col_2: %zu\n", row_2, col_2);
		fprintf(stderr, "same_dual_possibility: At least one cell doesn't have 2 possibilities\n");
		exit(EXIT_FAILURE);
	}
#endif

	for (unsigned val=MIN_VALUE; val<=MAX_VALUE; val++)
	{
		if (possible_values[row_1][col_1].possible[val] == true &&
		    possible_values[row_2][col_2].possible[val] == true)
		{
			if (val_1 == 0)
			{
				val_1 = val;
			}
			else
			{
				val_2 = val;
			}
		}
	}

	if (val_1 != 0 && val_2 != 0)
	{
		doubles.vals[0] = val_1;
		doubles.vals[1] = val_2;
	}

	return doubles;
}

/**
  * Used to avoid redundancy in the 'update_possibilities_1' function.
  */
static void update_possibilities_1_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
					  struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
					  size_t row_1, size_t col_1,
					  size_t row_2, size_t col_2,
					  struct naked_double_values doubles,
					  size_t search_row_start, size_t search_row_end,
					  size_t search_col_start, size_t search_col_end)
{

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES_1
	printf("update_possibilities_1_helper: requested for updating possibilities for 'naked double':\n");
	printf("update_possibilities_1_helper: row_1: %zu, col_1: %zu; row_2: %zu, col_2: %zu\n",
		row_1, col_1, row_2, col_2);
	printf("update_possibilities_1_helper: Updating possibilities in range:\n");
	printf("update_possibilities_1_helper: search_row_start: %zu\t search_row_end: %zu\n"
		"update_possibilities_1_helper:search_col_start: %zu\t search_col_end: %zu\n",
		search_row_start, search_row_end, search_col_start, search_col_end);
	printf("\n");
#endif

	for (size_t search_row=search_row_start; search_row<=search_row_end; search_row++)
	{
		for (size_t search_col=search_col_start; search_col<=search_col_end; search_col++)
		{
			// ignore the "naked double" cells
			if (
			    (search_row == row_1 && search_col == col_1) ||
			    (search_row == row_2 && search_col == col_2)
			)
			{

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES_1
				printf("update_possibilities_1_helper: skipping row: %zu, col: %zu\n",
					search_row, search_col);
#endif

				continue;
			}

			for (size_t val_index=0; val_index<2; val_index++)
			{
				const unsigned val = doubles.vals[val_index];
				if (sudoku_table[search_row][search_col] == 0 &&
				possible_values[search_row][search_col].possible[val] != false)
				{
					possible_values[search_row][search_col].possible[val] = false;
					possible_values[search_row][search_col].possibilities--;
					if (possible_values[search_row][search_col].possibilities == 1)
					{

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES_1
						printf("update_possibilities_1_helper: only_possibility: %u for row: %zu, col: %zu\n",
							find_naked_single(possible_values, search_row, search_col),
							search_row, search_col);
#endif

						insert_naked_single(search_row, search_col);
					}

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES_1
					else if (possible_values[search_row][search_col].possibilities == 0)
					{
						fprintf(stderr, "update_possibilities_1_helper: incorrect move.\n");
						fprintf(stderr, "update_possibilities_1_helper: updating row: "
							"%zu, col: %zu with %u\n", search_row, search_row, val);
						fprintf(stderr, "update_possibilities_1_helper: left "
							"row: %zu, col: %zu with no possibilities\n",
							search_row, search_col);
						print_table(sudoku_table);
						exit(EXIT_FAILURE);
					}
					else
					{
						printf("update_possibilities_1_helper: %u possibilities for row: %zu, col: %zu\n",
							possible_values[search_row][search_col].possibilities,
							search_row, search_col);
					}
#endif

				}
			}
		}
	}

#ifdef KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES_1
	printf("\n");
#endif
}

void update_possibilities_1(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			    struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			    size_t row_1, size_t col_1,
			    size_t row_2, size_t col_2,
			    struct naked_double_values doubles)
{
	if (row_1 == row_2)
	{
		// "naked double" in a row
		update_possibilities_1_helper(sudoku_table,possible_values,
					      row_1, col_1, row_2, col_2, doubles,
					      row_1, row_1, 0, TABLE_ORDER_MAX-1);
	}
	else if (col_1 == col_2)
	{
		// "naked double" in a column
		update_possibilities_1_helper(sudoku_table, possible_values,
					      row_1, col_1, row_2, col_2, doubles,
					      0, TABLE_ORDER_MAX-1, col_1, col_1);
	}
	else
	{
		// find corners of square
		const size_t top_left_row = (row_1/3)*3;
		const size_t top_left_col = (col_1/3)*3;

		// "naked double" in a square
		update_possibilities_1_helper(sudoku_table, possible_values,
					      row_1, col_1, row_2, col_2, doubles,
					      top_left_row, top_left_row+SQUARE_DIMENSION-1,
					      top_left_col, top_left_col+SQUARE_DIMENSION-1);
	}
}

/**
  * Specifies the amount by which the row must be offset to search for the next naked double
  * in the given search space.
  *
  * Returns a 0 or 1 depending on the dimensions of the search space.
  */
static size_t find_row_offset(size_t search_row_start, size_t search_row_end,
			      size_t search_col_start, size_t search_col_end)
{
	if (search_row_start==search_row_end)
	{
		return 0; // searching a naked double in a row
	}
	else if (search_col_start != search_col_end)
	{
		return 0; // searching a naked double in a square
	}
	else
	{
		return 1; // searching a naked double in a column
	}
}

bool solve_naked_doubles_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 size_t search_row_start, size_t search_row_end,
				 size_t search_col_start, size_t search_col_end)
{

#ifdef KS_SUDOKU_DEBUG_NAKED_DOUBLE_SEARCH
	printf("solve_naked_doubles_helper: request to search for a 'naked double' in range:\n");
	printf("solve_naked_doubles_helper: search_row_start: %zu, search_row_end: %zu\n", search_row_start, search_row_end);
	printf("solve_naked_doubles_helper: search_col_start: %zu, search_col_end: %zu\n", search_col_start, search_col_end);
#endif

	bool found_naked_double = false;

	for (size_t first_double_search_row=search_row_start; first_double_search_row<=search_row_end; first_double_search_row++)
	{
		for (size_t first_double_search_col=search_col_start; first_double_search_col<=search_col_end; first_double_search_col++)
		{
			if (sudoku_table[first_double_search_row][first_double_search_col] == 0 &&
			    possible_values[first_double_search_row][first_double_search_col].possibilities == 2)
			{
				const size_t next_row_offset = find_row_offset(search_row_start, search_row_end,
										search_col_start, search_col_end);
				const size_t next_col_offset = 1-next_row_offset; // always the complement of 'next_row_offset'

#ifdef KS_SUDOKU_DEBUG_NAKED_DOUBLE_SEARCH
				printf("solve_naked_doubles_helper: found a double possibility at row: %zu, col: %zu\n",
					first_double_search_row, first_double_search_col);
#endif

				size_t second_double_search_row = first_double_search_row+next_row_offset,
				       second_double_search_col = first_double_search_col+next_col_offset;

				for (;
				     second_double_search_row<=search_row_end;
				     second_double_search_row++)
				{
					for (;
					     second_double_search_col<=search_col_end;
					     second_double_search_col++)
					{

#ifdef KS_SUDOKU_DEBUG_NAKED_DOUBLE_SEARCH
						printf("solve_naked_doubles_helper: searching for a double possibility "
							"at row: %zu, col: %zu\n",
							second_double_search_row, second_double_search_col);
#endif
						if (sudoku_table[second_double_search_row][second_double_search_col] == 0 &&
						    possible_values[second_double_search_row][second_double_search_col].possibilities == 2)
						{
							const struct naked_double_values d = same_dual_possibility(possible_values,
												first_double_search_row,
												first_double_search_col,
												second_double_search_row,
												second_double_search_col);

							if (d.vals[0] !=0 && d.vals[1] != 0)
							{

#ifdef KS_SUDOKU_DEBUG
								printf("solve_naked_doubles: found naked double at\n"
									"solve_naked_doubles: row_1: %zu, col_1: %zu\n"
									"solve_naked_doubles: row_2: %zu, col_2: %zu\n"
									"solve_naked_doubles: with values val_1: %u, val_2 :%u\n",
									first_double_search_row, first_double_search_col,
									second_double_search_row, second_double_search_col,
									d.vals[0], d.vals[1]);
#endif

								update_possibilities_1(sudoku_table, possible_values,
											first_double_search_row, first_double_search_col,
											second_double_search_row, second_double_search_col,
											d);

								found_naked_double = true;
							}
						}
					}

					second_double_search_col = search_col_start; // to ensure the search in square is done correctly
				}
			}
		}
	}

	return found_naked_double;
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

#ifdef KS_SUDOKU_DEBUG
void print_possibility_vector(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			      struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			if (sudoku_table[row][col] == 0)
			{
				printf("print_possibility_vector: %u possible values for row: %zu, col: %zu\n",
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
#endif
