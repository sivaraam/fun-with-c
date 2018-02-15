/**
  * A simple sudoku solver
  */
#ifndef KS_SUDOKU_SOLVER
#define KS_SUDOKU_SOLVER

#define TABLE_ORDER_MAX 9

/**
  * Assumptions:
  *
  *   - 9*9 solvable sudoku table
  *   - empty cells are represneted by '0'
  *   - no invalid table entries (i.e.) table entries are in range [0, 9]
  *
  * This is not a program that can solve every single sudoku table in the wild.
  * It currently tries to solve sudoku using the following techniques,
  *
  *    - Naked single detection
  *    - Hidden single detection
  *
  * Note: The resulting solved table is stored in the table given as input.
  */
void solve_sudoku(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX]);

#endif
