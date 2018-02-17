#ifndef KS_SUDOKU_SOLVER_SOLVER_HELPERS
#define KS_SUDOKU_SOLVER_SOLVER_HELPERS

#include <sys/types.h>
#include <stddef.h>
#include "common.h"
#include "sudoku_solver.h"

/**
  * Returns the "naked single" value for the given cell.
  */
unsigned find_naked_single(struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			   size_t row, size_t col);

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
			  size_t row, size_t col, unsigned val);

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
				 unsigned val);

/*
 * Update the possibilities of cells as a consequence of detecting a naked double pair
 * {(row_1, col_1), (row_2, col_2)}.
 *
 *  sudoku_table - the table containing the sudoku board
 *  possible_values - lookup table for possible values of different cells in the
 *                    sudoku board.
 *  (row_1, col_1; row_2, col_2) - the naked double pair due to which the update is being performed
 *  doubles - the values in the naked double pair
 */
void update_possibilities_1(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			  struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
			  size_t row_1, size_t col_1,
			  size_t row_2, size_t col_2,
			  struct naked_double_values doubles);

/**
  * Helper function to avoid redundancy in the 'solve_naked_doubles' function.
  *
  * sudoku_table - the table containing the sudoku board
  * possible_values - lookup table for possible values of different cells in the
  *                    sudoku board.
  * (search_row_start, search_row_end) - the range of rows which must be searched
                                          for a "naked double"
  * (search_col_start, search_col_end) - the range of colums which must be searched
                                          for a "naked double"
  *
  * Returns true if a "naked double" was found in the search space.
  */
bool solve_naked_doubles_helper(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
				 size_t search_row_start, size_t search_row_end,
				 size_t search_col_start, size_t search_col_end);

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
				size_t row, size_t col);

/**
  * Finds a cell with two possibilities in a given row and returns the corresponding column.
  * If no such cell is found returns -1.
  * The search starts from 'col'.
  */
ssize_t find_double(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
		    struct possible_entries possible_values[TABLE_ORDER_MAX][TABLE_ORDER_MAX],
		    size_t row, size_t col);
#endif
