/**** DEV ****/
#define KS_SUDOKU_DEBUG
/**** DEV *****/

#ifdef KS_SUDOKU_DEBUG
#include <stdio.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/queue.h>

#include "sudoku_solver.h"

// Type to hold the possible values for a cell (identified by row and column)
struct possible_entries
{
	bool possible[MAX_VALUE+1]; // an index is set to 'true' if the corresponding value is possible
	unsigned possibilities; // the number of possible values for a cell
};

struct forced_cell {
	size_t row, col;
        STAILQ_ENTRY(forced_cell) entries;     /* Singly-linked List. */
};

// intialise the list used to store the cells to manipulate first
STAILQ_HEAD(slisthead, forced_cell) head = STAILQ_HEAD_INITIALIZER(head);

/**
  * Initialise 'possible_entries' with values that are possible for a given sudoku cell.
  * Obviously the cell is expected not to be an already filled one.
  *
  * Done by linearly searching the row, the column and the square.
  *
  * Returns the number of possible values.
  */
unsigned initialise_possible_values(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX], struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX], size_t row, size_t col)
{
	unsigned possibilities = NUMBER_OF_VALUES;

	// initialise all values to 'true'
	for (size_t val=MIN_VALUE; val<=MAX_VALUE; val++)
	{
		possible_values[row][col].possible[val] = true;
	}

	// search the row to find the values which are not possible
	for (size_t search_col=0; search_col<TABLE_ORDER_MAX; search_col++)
	{
		if (search_col == col)
		{
			continue;
		}

		const unsigned val = sudoku_table[row][search_col];
		if (val != 0)
		{
			possible_values[row][col].possible[val] = false;
			possibilities--;
		}	
	}

	// search the column to find the values which are not possible
	for (size_t search_row=0; search_row<TABLE_ORDER_MAX; search_row++)
	{
		if (search_row == row)
		{
			continue;
		}

		if (sudoku_table[search_row][col] != 0)
		{
			const unsigned val = sudoku_table[search_row][col];
			if (possible_values[row][col].possible[val] != false) // avoid multiple counting
			{
				possible_values[row][col].possible[val] = false;
				possibilities--;
			}
		}	
	}

	// find corners of square
	const size_t top_left_row = (row/3)*3;
	const size_t top_left_col = (col/3)*3;

	// search the remaining cells of the square for values which are not possible
	for (size_t search_row=top_left_row; search_row<top_left_row+3; search_row++)
	{
		for (size_t search_col=top_left_col; search_col<top_left_col+3; search_col++)
		{
			if (search_row == row || search_col == col)
			{
				continue;
			}

			if (sudoku_table[search_row][search_col] != 0)
			{
				const unsigned val = sudoku_table[search_row][search_col];
				if (possible_values[row][col].possible[val] != false) // avoid multiple counting
				{
					possible_values[row][col].possible[val] = false;
					possibilities--;
				}
			}
		}
	}

	return possibilities;
}

void insert_forced_cell(size_t row, size_t col)
{
	struct forced_cell *n1 = malloc(sizeof(struct forced_cell));
	n1->row = row;	n1->col = col;
	STAILQ_INSERT_TAIL(&head, n1, entries);
}


// returns the only possible value for the given cell
unsigned find_fixed_possibility(struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX], size_t row, size_t col)
{
	for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
	{
		if (possible_values[row][col].possible[value] == true)
		{
			return value;
		}
	}
#ifdef KS_SUDOKU_DEBUG
	fprintf(stderr, "find_fixed_possibility: Invalid row(%zu) and column(%zu). Has %u possibilities.\n", row, col, possible_values[row][col].possibilities);
	for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
		fprintf(stderr, "%zu: %d\t", value, possible_values[row][col].possible[value]);
	exit(EXIT_FAILURE);
#endif
//	return 0; // for safety (NOTE YET)
	
}

// update the possibilities of cells as a consequence of the assignment of 'val' to the given cell
void update_possibilities(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX], struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX], size_t row, size_t col, unsigned val)
{
	// update the cells in the same row
	for (size_t search_col=0; search_col<TABLE_ORDER_MAX; search_col++)
	{
		if (search_col == col)
		{
			continue;
		}

		if (sudoku_table[row][search_col] == 0 && possible_values[row][search_col].possible[val] != false)
		{
			possible_values[row][search_col].possible[val] = false;
			possible_values[row][search_col].possibilities--;
			if (possible_values[row][search_col].possibilities == 1)
			{
#ifdef KS_SUDOKU_DEBUG
				fprintf(stderr, "update_possibilities: only_possibility: %u\n", find_fixed_possibility(possible_values, row, search_col));
#endif
				insert_forced_cell(row, search_col);
			}
#ifdef KS_SUDOKU_DEBUG
			else if (possible_values[row][search_col].possibilities == 0)
			{
				fprintf(stderr, "update_possibilities: incorrect move by updating %zuth row and %zuth col with %u\n", row, col, val);
				fprintf(stderr, "update_possibilities: it left %zuth row and %zuth column with no possibilities\n", row, search_col);
				exit(EXIT_FAILURE);
			}
			else
			{
				fprintf(stderr, "update_possibilities(row cell): %u possibilities for %zuth row and %zuth column\n", possible_values[row][search_col].possibilities, row, search_col);
			}
#endif
		}
	}

	// update the cells in the same column
	for (size_t search_row=0; search_row<TABLE_ORDER_MAX; search_row++)
	{
		if (search_row == row)
		{
			continue;
		}

		if (sudoku_table[search_row][col] == 0 && possible_values[search_row][col].possible[val] != false)
		{
			possible_values[search_row][col].possible[val] = false;
			possible_values[search_row][col].possibilities--;
			if (possible_values[search_row][col].possibilities == 1)
			{
#ifdef KS_SUDOKU_DEBUG
				fprintf(stderr, "update_possibilities: only_possibility: %u\n", find_fixed_possibility(possible_values, search_row, col));
#endif
				insert_forced_cell(search_row, col);
			}
#ifdef KS_SUDOKU_DEBUG
			else if (possible_values[search_row][col].possibilities == 0)
			{
				fprintf(stderr, "update_possibilities: incorrect move by updating %zuth row and %zuth col with %u\n", row, col, val);
				fprintf(stderr, "update_possibilities: it left %zuth row and %zuth column with no possibilities\n", search_row, col);
				exit(EXIT_FAILURE);
			}
			else
			{
				fprintf(stderr, "update_possibilities(column cell): %u possibilities for %zuth row and %zuth column\n", possible_values[search_row][col].possibilities, search_row, col);
			}
#endif
		}
	}

	// find corners of square
	const size_t top_left_row = (row/3)*3;
	const size_t top_left_col = (col/3)*3;

	// search the remaining cells of the square for values which are not possible
	for (size_t search_row=top_left_row; search_row<top_left_row+3; search_row++)
	{
		for (size_t search_col=top_left_col; search_col<top_left_col+3; search_col++)
		{
			if (search_row == row || search_col == col)
			{
				continue;
			}

			if (sudoku_table[search_row][search_col] == 0 && possible_values[search_row][search_col].possible[val] != false)
			{
				possible_values[search_row][search_col].possible[val] = false;
				possible_values[search_row][col].possibilities--;
				if (possible_values[search_row][col].possibilities == 1)
				{
#ifdef KS_SUDOKU_DEBUG
				fprintf(stderr, "update_possibilities: only_possibility: %u\n", find_fixed_possibility(possible_values, search_row, search_col));
#endif
					insert_forced_cell(search_row, col);
				}
#ifdef KS_SUDOKU_DEBUG
				else if (possible_values[search_row][search_col].possibilities == 0)
				{
					fprintf(stderr, "update_possibilities: incorrect move by updating %zuth row and %zuth col with %u\n", row, col, val);
					fprintf(stderr, "update_possibilities: it left %zuth row and %zuth column with no possibilities\n", search_row, search_col);
					exit(EXIT_FAILURE);
				}
				else
				{
					fprintf(stderr, "update_possibilities(other cell): %u possibilities for %zuth row and %zuth column\n", possible_values[search_row][search_col].possibilities, search_row, search_col);
				}
#endif
			}
		}
	}

}

void solve(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX], struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	while (!STAILQ_EMPTY(&head))
	{
		struct forced_cell *curr = STAILQ_FIRST(&head);
		STAILQ_REMOVE_HEAD(&head, entries);

		unsigned fixed_possibility = find_fixed_possibility(possible_values, curr->row, curr->col);
		sudoku_table[curr->row][curr->col] = fixed_possibility;
		possible_values[curr->row][curr->col].possibilities--; // not much use but anyways as it zeros the result
		update_possibilities(sudoku_table, possible_values, curr->row, curr->col, fixed_possibility);

#ifdef KS_SUDOKU_DEBUG
		printf("solve: fixed possibility: %u\n", fixed_possibility);
		struct forced_cell *print_curr = STAILQ_FIRST(&head);
		printf("solve: Manipulate list entries:\n");
		while (print_curr != NULL)
		{
			printf("%zu\t%zu\n", print_curr->row, print_curr->col);
			print_curr = STAILQ_NEXT(print_curr, entries);
		}
		printf("\n\n");
#endif
		free(curr);
	}
}

void solve_sudoku(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
	// the lookup table used to identify the possibilities of different cells
	struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX];

	STAILQ_INIT(&head);

#ifdef KS_SUDOKU_DEBUG
	printf("\n");
	printf("solve_sudoku: SUDOKU table obtained as input\n");
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			printf("%u\t", sudoku_table[row][col]);
		}
		printf("\n\n");
	}
#endif
	for (size_t row=0; row<TABLE_ORDER_MAX; row++)
	{
		for (size_t col=0; col<TABLE_ORDER_MAX; col++)
		{
			if (sudoku_table[row][col] == 0)
			{
				// initialise the possible values
				possible_values[row][col].possibilities = initialise_possible_values(sudoku_table, possible_values, row, col);
#ifdef KS_SUDOKU_DEBUG
				printf("Possible values for %zuth row and %zuth column: %u\n", row, col, possible_values[row][col].possibilities);
				for (size_t value=MIN_VALUE; value<=MAX_VALUE; value++)
				{
					if (possible_values[row][col].possible[value] == true)
					{
						printf("%zu\t", value);
					}
				}
				printf("\n\n");
#endif

				if (possible_values[row][col].possibilities == 1)
				{
					insert_forced_cell(row, col);
				}
			}
		}
	}
#ifdef KS_SUDOKU_DEBUG
	struct forced_cell *curr = STAILQ_FIRST(&head);
	printf("Manipulate list entries:\n");
	while (curr != NULL)
	{
		printf("%zu\t%zu\n", curr->row, curr->col);
		curr = STAILQ_NEXT(curr, entries);
	}
	printf("\n\n");
#endif
	solve(sudoku_table, possible_values);
}
