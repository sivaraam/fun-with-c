/**
  * A solver for an easy sudoku question
  */
#ifndef KS_SUDOKU_SOLVER
#define KS_SUDOKU_SOLVER

#define MIN_VALUE 1
#define MAX_VALUE 9
#define NUMBER_OF_VALUES 9
#define TABLE_ORDER_MAX 9

/**
  * Assumptions:
  *
  *   - 9*9 solvable sudoku table
  *   - empty cells are represneted by '0'
  *   - no invalid table entries (i.e.) table entries are in range [0, 9]
  *   - table is solvable using "forced cell" moves alone
  *
  * Note: The resulting solved table is stored in the table given as input.
  */
void solve_sudoku(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX]);

#endif
