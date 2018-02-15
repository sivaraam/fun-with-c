#ifndef KS_SUDOKE_SOLVER_COMMON
#define KS_SUDOKE_SOLVER_COMMON

#include <stdbool.h>
#include "sudoku_solver.h"

#define MIN_VALUE 1
#define MAX_VALUE 9
#define NUMBER_OF_VALUES 9
#define SQUARE_DIMENSION 3

/**** DEV ****/
#define KS_SUDOKU_DEBUG
//#define KS_SUDOKU_DEBUG_UPDATE_POSSIBILITIES
//#define KS_SUDOKU_DEBUG_HIDDEN_SINGLE_SEARCH
/**** DEV *****/

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

void print_table(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX]);

#endif
