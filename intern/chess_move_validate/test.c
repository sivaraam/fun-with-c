#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "chess.h"

#define VERBOSE

square_index_type normalize_row (int row)
{
	if (row <= CHAR_MAX &&
	    row >= '1' &&
	    row <='8')
	{
		return row - '1';
	}
	else
	{
		fprintf (stderr, "Invalid row: %d\n", row);
		exit (EXIT_FAILURE);
	}
}

square_index_type normalize_col (int col)
{
	if (col <= CHAR_MAX &&
	    (
	     (col >= 'a' &&
	      col <= 'h') ||
	     (col >= 'A' &&
	      col <= 'H')
	    )
	   )
	{
		if (isupper (col))
		{
			return col - 'A';
		}
		else
		{
			return col - 'a';
		}
	}
	else
	{
		fprintf (stderr, "Invlid col: %d\n", col);
		exit(EXIT_FAILURE);
	}
}

int main (void)
{
	struct chess *game = get_new_game();

	printf ("Welcome to the virtual chess!\n");
	printf ("A new game has been initialised\n");
	printf ("Enter the moves to make:");

	while (1)
	{

#ifdef VERBOSE
	printf ("\n");
	for (square_index_type row = 0; row < game->board_limits.row; row++)
	{
		for (square_index_type col = 0; col < game->board_limits.col; col++)
		{
				printf ("%p\t", (void *)*(*(game->board + row) + col));
		}
		printf ("\n");
	}
#endif

		int src_row = 0, src_col = 0, trailing = 0,
		    dest_row, dest_col = 0;
		src_col = getchar();
		src_row = getchar();
		if (src_row == EOF ||
		    src_col == EOF)
		{
			printf ("Error in reading input!\n");
			return 1;
		}

		// throw away the trailing char if any
		trailing = getchar();
		if (!isspace (trailing))
		{
			ungetc (trailing, stdin);
		}

		dest_col = getchar();
		dest_row = getchar();
		if (dest_row == EOF ||
		    dest_col == EOF)
		{
			printf ("Error in reading input!\n");
			return 1;
		}

		// throw away the trailing char if any
		trailing = getchar();
		if (!isspace (trailing))
		{
			ungetc (trailing, stdin);
		}

		square_index_type normalized_src_row = normalize_row (src_row),
		                  normalized_src_col = normalize_col (src_col),
		                  normalized_dest_row = normalize_row (dest_row),
		                  normalized_dest_col = normalize_col (dest_col);
		int ret;

		printf ("Normalized values: src_row: %d src_col: %d\n",
			normalized_src_row,
			normalized_src_col);
		printf ("Normalized values: dest_row: %d dest_col: %d\n",
			normalized_dest_row,
			normalized_dest_col);
		if (*(*(game->board + normalized_src_row) + normalized_src_col) == NULL)
		{
			printf ("No coin found at source!\n");
			continue;
		}

		if ((ret = move_coin (game, normalized_src_row, normalized_src_col, normalized_dest_row, normalized_dest_col)) > 0 )
		{
			printf ("Move success with code: %d\n", ret);
		}
		else if (ret == ERR_TURN)
		{
			printf ("It's not your turn!\n");
		}
		else
		{
			printf ("Move failed with code: %d\n", ret);
		}
	}
}
