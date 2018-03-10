#include <stdlib.h>
#include "common.h"
#include "chess.h"
#include "chess_helpers.h"

#ifdef KS_CHESS_DEBUG
#   include <stdio.h>
#endif

struct chess *get_new_game (void)
{
	static const
	struct chess_position initial_board_limits = { 8, 8 };

	/** Memory acquisistion phase **/
	struct chess *new_game = malloc (sizeof (struct chess));

	if (new_game == NULL)
	{
		return NULL;
	}

	new_game->board = malloc (sizeof (struct chess_coin**) * initial_board_limits.row);

	if (new_game->board == NULL)
	{
		free (new_game);
		return NULL;
	}

	for (square_index_type row = 0; row < initial_board_limits.row; row++)
	{
		*(new_game->board + row) = malloc (sizeof (struct chess_coin*) * initial_board_limits.col);

		if (*(new_game->board + row) == NULL)
		{
			// free memory of already allocated square pointers
			for (square_index_type row_to_free = 0; row_to_free < row; row_to_free++)
			{
			    free (*(new_game->board + row_to_free));
			}

			// free memory for the board and the game
			free (new_game->board);
			free (new_game);

			return NULL;
		}
	}

	/** Initialisation phase **/
	new_game->is_whites_turn = 1;
	new_game->board_limits = initial_board_limits;

	static const
	square_index_type white_first_row = 0,
	                  black_first_row = 6,
	                  white_rows = 2,
	                  black_rows = 2;
	square_index_type curr_row = white_first_row, curr_col = 0;

	/* Initialise the coins for white */
	if (
		(chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_ROOK)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_KNIGHT)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_BISHOP)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_QUEEN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_KING)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_BISHOP)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_KNIGHT)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_ROOK)) ||

		(curr_row++, curr_col = 0, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), WHITE_PAWN))
	)
	{
		goto MEMORY_FAILED;
	}
#ifdef KS_CHESS_DEBUG
	else
	{
		printf ("get_new_game: Successfully created white coin cells\n");
	}

	printf ("get_new_game: curr_row: %d, curr_col: %d", curr_row, curr_col);
#endif

	/* Initialise squares in between with NULL */
	for (curr_row++; curr_row < black_first_row; curr_row++)
	{
		for (curr_col = 0; curr_col < new_game->board_limits.col; curr_col++)
		{
			*(*(new_game->board + curr_row) + curr_col) = NULL;
		}
	}

	/* Initialise the coins for black */
	curr_col = 0;

	if (
		(chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_PAWN)) ||

		(curr_row++, curr_col = 0, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_ROOK)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_KNIGHT)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_BISHOP)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_QUEEN)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_KING)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_BISHOP)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_KNIGHT)) ||
		(curr_col++, chess_coin_init_helper (curr_row, curr_col, &(*(*(new_game->board + curr_row) + curr_col)), BLACK_ROOK))
	)
	{
		goto MEMORY_FAILED;
	}

#ifdef KS_CHESS_DEBUG
	else
	{
		printf ("get_new_game: Successfully created black coin cells\n");
	}
#endif

	/** Initialise the possible moves for the white coins **/
	for (square_index_type row = white_first_row; row < white_rows; row++)
	{
		for (square_index_type col = 0; col < new_game->board_limits.col; col++)
		{

#ifdef KS_CHESS_GET_NEW_GAME_INITIALISE_DEBUG
			printf ("get_new_game: Initialising possibilities for white coin at row: %d, col: %d\n", row, col);
#endif

			initialise_possibilities (new_game, row, col);
		}
	}

	/** Initialise the possible moves for the black coins **/
	for (square_index_type row = black_first_row; row < black_first_row+black_rows; row++)
	{
		for (square_index_type col = 0; col < new_game->board_limits.col; col++)
		{

#ifdef KS_CHESS_GET_NEW_GAME_INITIALISE_DEBUG
			printf ("get_new_game: Initialising possibilities for black coin at row: %d, col: %d\n", row, col);
#endif

			initialise_possibilities (new_game, row, col);
		}
	}

	return new_game;

MEMORY_FAILED:
	// free memory of already allocated chess coins
	for (square_index_type col_to_free = 0; col_to_free < curr_col; col_to_free++)
	{
		free (*(new_game->board + col_to_free));
	}

	// free memory for the board and the game
	free (new_game->board);
	free (new_game);

	return NULL;
}

int move_coin (struct chess *game,
               square_index_type src_row,
               square_index_type src_col,
               square_index_type dest_row,
               square_index_type dest_col)
{
	if (game == NULL)
	{
		return ERR_NULL;
	}

	struct chess_coin *const src_coin = *(*(game->board + src_row) + src_col);
	struct chess_coin *const dest_coin = *(*(game->board + dest_row) + dest_col);

	if (src_coin == NULL)
	{
		return ERR_NULL;
	}

	if (
	    (game->is_whites_turn && src_coin->colour != WHITE_COIN) ||
	    (!game->is_whites_turn && src_coin->colour != BLACK_COIN)
	   )
	{
		return ERR_TURN;
	}

#ifdef KS_CHESS_DEBUG
	printf ("move_coin: Moving coin from (%d, %d) to (%d, %d)\n", src_row, src_col, dest_row, dest_col);
#endif

	enum move_type ret_val = MOVE_ILLEGAL;
	unsigned possible_move = 0;

#ifdef KS_CHESS_DEBUG
	printf ("move_coin: Possible moves for source: %d\n", src_coin->possible_moves_num);
#endif

	// Check if the destination position is a valid move for the source coin.
	for (;
	     possible_move < src_coin->possible_moves_num;
	     possible_move++)
	{
		struct chess_position *curr_possible_pos = src_coin->possible_moves + possible_move;

#ifdef KS_CHESS_MOVE_COIN_DEBUG
			printf ("move_coin: Coin has possible move to (%d, %d)\n",
			        curr_possible_pos->row, curr_possible_pos->col);
#endif

		if (curr_possible_pos->row == dest_row &&
		    curr_possible_pos->col == dest_col)
		{


			if (dest_coin != NULL)
			{
				ret_val = MOVE_TAKEDOWN;

#ifdef KS_CHESS_MOVE_COIN_DEBUG
				printf ("move_coin: Destination (%d, %d) is a 'take down' move\n", dest_row, dest_col);
#endif

			}
			else
			{

#ifdef KS_CHESS_MOVE_COIN_DEBUG
				printf ("move_coin: Destination (%d, %d) is a 'occupy' move\n", dest_row, dest_col);
#endif

				ret_val = MOVE_OCCUPY;
			}
			break;
		}
	}

	if (possible_move == src_coin->possible_moves_num)
	{
		return MOVE_ILLEGAL;
	}

	// Move the coin to its destination
	const struct chess_position dest_pos = { dest_row, dest_col };
	src_coin->position = dest_pos;
	*(*(game->board + dest_row) + dest_col) = src_coin;
	*(*(game->board + src_row) + src_col) = NULL;
	free (dest_coin);

	// unset first move flag if coin is a pawn
	if (src_coin->type == PAWN &&
	    src_coin->is_pawns_first_move)
	{
		src_coin->is_pawns_first_move = 0;
	}

#ifdef KS_CHESS_MOVE_COIN_DEBUG
	printf ("move_coin: Successfully moved coin from source to destination\n");
#endif

	// Update the possibilities of all coins in the board
	// as a consequence of the new move.
	for (square_index_type row = 0; row < game->board_limits.row; row++)
	{
		for (square_index_type col = 0; col < game->board_limits.col; col++)
		{
			if (*(*(game->board + row) + col) != NULL)
			{

#ifdef KS_CHESS_MOVE_COIN_DEBUG
				printf ("move_coin: Updating possibilities for coin at (%d, %d)\n", row, col);

				if (update_possibilities (game, row, col))
				{
					fprintf (stderr, "move_coin: Updating possibilities failed for (%d, %d)\n", row, col);
					exit (EXIT_FAILURE);
				}
#else
				update_possibilities (game, row, col);
#endif

			}
		}
	}

	// Toggle the turn state
	game->is_whites_turn = !game->is_whites_turn;

	return ret_val;

}
