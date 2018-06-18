#include <stdio.h>
#include "common.h"

void print_table(unsigned sudoku_table[TABLE_ORDER_MAX][TABLE_ORDER_MAX])
{
       for (size_t row=0; row<TABLE_ORDER_MAX; row++)
       {
               for (size_t col=0; col<TABLE_ORDER_MAX; col++)
               {
                       printf("%u\t", sudoku_table[row][col]);
               }
               printf("\n\n");
       }
}
