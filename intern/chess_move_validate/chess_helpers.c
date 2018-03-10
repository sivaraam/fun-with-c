#include <stdlib.h>
#include "common.h"
#include "chess_helpers.h"

#ifdef KS_CHESS_DEBUG
#    include <stdio.h>
#endif

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
#    include <stddef.h>
#endif

/**
 * is_valid_position:
 *
 * @pos: The position in the game board
 *
 * Helper function used to validate whether the given position represents
 * valid position for the given board dimensions.
 *
 * Returns: Non-zero value if the position is valid else returns zero.
 */
static
int is_valid_position (const struct chess_position pos,
                       const struct chess_position board_limits)
{
	if (pos.row >= 0 && pos.row < board_limits.row &&
	    pos.col >= 0 && pos.col < board_limits.col)
	{
		return 1;
	}

	return 0;
}

int initialise_possibilities (struct chess *const game,
                             square_index_type curr_coin_row,
                             square_index_type curr_coin_col)
{
	struct chess_coin *coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);

	if (coin_to_update == NULL)
	{
		return 1;
	}

	switch (coin_to_update->type)
	{
	case PAWN:
	{
		insert_possible_move (coin_to_update, curr_coin_row+(1*coin_to_update->colour), curr_coin_col);
		insert_possible_move (coin_to_update, curr_coin_row+(2*coin_to_update->colour), curr_coin_col);
	}
	break;

	case KNIGHT:
	{
		insert_possible_move (coin_to_update, curr_coin_row+(2*coin_to_update->colour), curr_coin_col+1);
		insert_possible_move (coin_to_update, curr_coin_row+(2*coin_to_update->colour), curr_coin_col-1);
	}
	break;
	}

	return 0;
}

/**
 * is_possible_move:
 *
 * @curr_coin: Pointer to the current coin for which 'move_pos' is being considered
 *             as a possible move.
 * @move_pos: Pointer to the coin position which is being considered as a possible
 *            destination for 'curr_coin'
 *
 * Helper function used avoid redundancy in 'update_*_possibilities' functions.
 *
 * Returns: Non-zero value if 'move_pos' could be considered as a valid position
 *          for 'curr_coin' where the type of 'curr_coin' is "not" a PAWN.
 */
static inline
int is_possible_move (const struct chess_coin *const curr_coin,
                      const struct chess_coin *const move_pos)
{
	if (
	    move_pos == NULL ||
	    (curr_coin->colour+move_pos->colour == 0)
	)
	{
		return 1;
	}

	return 0;
}

/**
 * update_pawn_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Updates the possible positions to which the coin in the given position could move
 * considering it to be a pawn.
 *
 * Note:
 * 1. A valid coin is expected to be found in the given position.
 * 2. The possibility vector must be empty i.e., the previous possibilities if any should be truncated.
 */
static
void update_pawn_possibilities (const struct chess *const game,
                                const square_index_type curr_coin_row,
                                const square_index_type curr_coin_col)
{
	struct chess_coin *const coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_pawn_possibilities: Updating pawn possibilities for (%d, %d)\n", curr_coin_row, curr_coin_col);
#endif

	struct chess_position next_cell_pos = {
		curr_coin_row + coin_to_update->colour,
		curr_coin_col
	};

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_pawn_possibilities: Next cell possibility: (%d, %d)\n", next_cell_pos.row, next_cell_pos.col);
	printf ("update_pawn_possibilities: Next cell value: %p\n", *(*(game->board + next_cell_pos.row) + next_cell_pos.col));
#endif

	const
	struct chess_position take_down_cells_pos[] = {
		{ next_cell_pos.row, curr_coin_col - 1 },
		{ next_cell_pos.row, curr_coin_col + 1 }
	};

	// check if straight move is possible
	if (is_valid_position (next_cell_pos, game->board_limits) &&
	    *(*(game->board + next_cell_pos.row) + next_cell_pos.col) == NULL
	)
	{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_pawn_possibilities: Found next cell possibility.\n");
#endif

		insert_possible_move (coin_to_update, next_cell_pos.row, next_cell_pos.col);
	}

	// check if double move possible if its the pawn's first move
	if (coin_to_update->is_pawns_first_move)
	{
		next_cell_pos.row = next_cell_pos.row + coin_to_update->colour;

		if (is_valid_position (next_cell_pos, game->board_limits) &&
		    *(*(game->board + next_cell_pos.row) + next_cell_pos.col) == NULL)
		{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
			printf ("update_pawn_possibilities: Found next cell possibility.\n");
#endif

			insert_possible_move (coin_to_update, next_cell_pos.row, next_cell_pos.col);
		}
	}

	// check if take-down moves are possible
	for (size_t curr_take_down = 0; curr_take_down < 2; curr_take_down++)
	{
		if (is_valid_position (take_down_cells_pos[curr_take_down], game->board_limits))
		{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
			printf ("update_pawn_possibilities: Checking for take down possibility (%d, %d)\n",
			        take_down_cells_pos[curr_take_down].row, take_down_cells_pos[curr_take_down].col);
#endif

			const
			struct chess_coin *take_down_cell = *(*(game->board +
			                                      take_down_cells_pos[curr_take_down].row) +
			                                      take_down_cells_pos[curr_take_down].col);

			if (take_down_cell != NULL &&
          (take_down_cell->colour+coin_to_update->colour == 0)
         )
			{
				insert_possible_move (coin_to_update, take_down_cells_pos[curr_take_down].row,
				                                      take_down_cells_pos[curr_take_down].col);
			}
		}
	}
}

/**
 * update_king_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Updates the possible positions to which the coin in the given position could move
 * considering it to be a king.
*
 * Note:
 * 1. A valid coin is expected to be found in the given position.
 * 2. The possibility vector must be empty i.e., the previous possibilities if any should be truncated.
 */
static
void update_king_possibilities (const struct chess *const game,
                                const square_index_type curr_coin_row,
                                const square_index_type curr_coin_col)
{
	struct chess_coin *const coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);
	const
	struct chess_position king_square_tl_corner = {
		(curr_coin_row-1 >= 0) ? curr_coin_row-1 : curr_coin_row,
		(curr_coin_col-1 >= 0) ? curr_coin_col-1 : curr_coin_col
	};
	const
	struct chess_position king_square_br_corner = {
		(curr_coin_row+1 < game->board_limits.row) ? curr_coin_row+1 : curr_coin_row,
		(curr_coin_col+1 < game->board_limits.col) ? curr_coin_col+1 : curr_coin_col
	};

	for (square_index_type king_square_row = king_square_tl_corner.row;
	     king_square_row <= king_square_br_corner.row;
	     king_square_row++)
	{
		for (square_index_type king_square_col = king_square_tl_corner.col;
		     king_square_col <= king_square_br_corner.col;
		     king_square_col++)
		{
			if (king_square_row == curr_coin_row &&
			    king_square_col == curr_coin_col)
			{
				continue;
			}

			const
			struct chess_coin *const king_square_coin = *(*(game->board + king_square_row) + king_square_col);
			if (is_possible_move (coin_to_update, king_square_coin))
			{
				insert_possible_move (coin_to_update, king_square_row, king_square_col);
			}
		}
	}
}

/**
 * update_knight_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Updates the possible positions to which the coin in the given position could move
 * considering it to be a knight.
 *
 * Note:
 * 1. A valid coin is expected to be found in the given position.
 * 2. The possibility vector must be empty i.e., the previous possibilities if any should be truncated.
 */
static
void update_knight_possibilities (const struct chess *const game,
                                  const square_index_type curr_coin_row,
                                  const square_index_type curr_coin_col)
{
	struct chess_coin *const coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);
	static const
	struct chess_position possible_knight_offsets[] = {
		{ -2, -1 },
		{ -2, +1 },
		{ -1, -2 },
		{ -1, +2 },
		{ +1, -2 },
		{ +1, +2 },
		{ +2, -1 },
		{ +2, +1 }
	};
	static const
	unsigned possible_knight_offsets_num = 8;

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_knight_possibilities: Updating possibilities for (%d, %d)\n", curr_coin_row, curr_coin_col);
#endif

	for (unsigned knight_offset = 0; knight_offset < possible_knight_offsets_num; knight_offset++)
	{
		const
		struct chess_position curr_knight_offset_pos = {
			curr_coin_row + possible_knight_offsets[knight_offset].row,
			curr_coin_col + possible_knight_offsets[knight_offset].col
		};

		if (is_valid_position (curr_knight_offset_pos, game->board_limits))
		{
			const
			struct chess_coin *const curr_knight_offset_coin = *(*(game->board +
			                                                     curr_knight_offset_pos.row) +
			                                                     curr_knight_offset_pos.col);

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_knight_possibilities: Considering possible move to (%d, %d)\n",
		        curr_knight_offset_pos.row, curr_knight_offset_pos.col);
		printf ("update_knight_possibilities: Val at move: %p\n", curr_knight_offset_coin);
#endif

			if (is_possible_move (coin_to_update, curr_knight_offset_coin))
			{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_knight_possibilities: Found possible move\n");
#endif

				insert_possible_move (coin_to_update,
				                      curr_knight_offset_pos.row,
				                      curr_knight_offset_pos.col);
			}
		}
	}
}

/**
 * update_rook_bishop_possibilities_helper:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @coin_to_update: The coin which is to be updated
 * @curr_possibility_pos: The position of the current possible move being considered
 *
 * Used to avoid redundancy in the 'update_rook_possibilities' and
 * the 'update_bishop_possibilities' functions.
 *
 * Returns: non-zero value if it finds a valid chess coin in the
 *          current possible position else return 0.
 */
static
int update_rook_bishop_possibilities_helper (const struct chess *const game,
                                             struct chess_coin *const coin_to_update,
                                             const struct chess_position curr_possibility_pos)
{
	const
	struct chess_coin *const curr_coin = *(*(game->board + curr_possibility_pos.row) + curr_possibility_pos.col);

	if (curr_coin == NULL)
	{
		// empty square
		insert_possible_move (coin_to_update, curr_possibility_pos.row, curr_possibility_pos.col);
		return 0;
	}
	else
	{
		if (coin_to_update->colour + curr_coin->colour == 0)
		{
			insert_possible_move (coin_to_update, curr_possibility_pos.row, curr_possibility_pos.col);
		}

		return 1;
	}
}

/**
 * update_rook_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Updates the possible positions to which the coin in the given position could move
 * considering it to be a rook.
 *
 * Note:
 * 1. A valid coin is expected to be found in the given position.
 * 2. The possibility vector must be empty i.e., the previous possibilities if any should be truncated.
 */
static
void update_rook_possibilities (const struct chess *const game,
                                const square_index_type curr_coin_row,
                                const square_index_type curr_coin_col)
{
	struct chess_coin *const coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_rook_possibilities: Updating rook possibilities for (%d, %d)\n", curr_coin_row, curr_coin_col);
#endif

	// search for possibilities in the left of the rook
	for (square_index_type left_square_col = coin_to_update->position.col - 1;
	     left_square_col >= 0;
	     left_square_col--)
	{
		const
		struct chess_position curr_left_pos = { coin_to_update->position.row, left_square_col };

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_left_pos))
		{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
			printf ("update_rook_possibilities: Found an end for left square possibilities at (%d, %d)\n",
			        curr_left_pos.row, curr_left_pos.col);
#endif

			break;
		}
	}

	// search for possibilities in the right of the rook
	for (square_index_type right_square_col = coin_to_update->position.col + 1;
	     right_square_col < game->board_limits.col;
	     right_square_col++)
	{
		const
		struct chess_position curr_right_pos = { coin_to_update->position.row, right_square_col };
	
		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_right_pos))
		{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
			printf ("update_rook_possibilities: Found an end for right square possibilities at (%d, %d)\n",
			        curr_right_pos.row, curr_right_pos.col);
#endif

			break;
		}
	}

	// search for possibilities to the top of the rook
	for (square_index_type top_square_row = coin_to_update->position.row - 1;
	     top_square_row >= 0;
	     top_square_row--)
	{
		const
		struct chess_position curr_top_pos = { top_square_row, coin_to_update->position.col };

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_top_pos))
		{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
			printf ("update_rook_possibilities: Found an end for top square possibilities at (%d, %d)\n",
			        curr_top_pos.row, curr_top_pos.col);
#endif

			break;
		}
	}

	// search for possibilities to the bottom of the rook
	for (square_index_type bottom_square_row = coin_to_update->position.row + 1;
	     bottom_square_row < game->board_limits.row;
	     bottom_square_row++)
	{
		const
		struct chess_position curr_bottom_pos = { bottom_square_row, coin_to_update->position.col };

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_bottom_pos))
		{

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
			printf ("update_rook_possibilities: Found an end for bottom square possibilities at (%d, %d)\n",
			        curr_bottom_pos.row, curr_bottom_pos.col);
#endif

			break;
		}
	}
}

/**
 * update_pawn_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Updates the possible positions to which the coin in the given position could move
 * considering it to be a bishop.
 *
 * Note:
 * 1. A valid coin is expected to be found in the given position.
 * 2. The possibility vector must be empty i.e., the previous possibilities if any should be truncated.
 */
static
void update_bishop_possibilities (const struct chess *const game,
                                  const square_index_type curr_coin_row,
                                  const square_index_type curr_coin_col)
{
	struct chess_coin *const coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_bishop_possibilities: Updating bishop possibilities for (%d, %d)\n", curr_coin_row, curr_coin_col);
#endif

	// search for possibilities in top left diagonal
	for (square_index_type top_left_row = coin_to_update->position.row - 1,
	                       top_left_col = coin_to_update->position.col - 1;
	     top_left_row >= 0 && top_left_col >= 0;
	     top_left_row--, top_left_col--)
	{
		const
		struct chess_position curr_top_left_pos = { top_left_row, top_left_col };

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_bishop_possibilities: Considering top left possibility: (%d, %d)\n",
		        curr_top_left_pos.row, curr_top_left_pos.col);
#endif

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_top_left_pos))
		{
			break;
		}
	}

	// search for possibilities in top right diagonal
	for (square_index_type top_right_row = coin_to_update->position.row - 1,
	                       top_right_col = coin_to_update->position.col + 1;
	     top_right_row >= 0 && top_right_col < game->board_limits.col;
	     top_right_row--, top_right_col++)
	{
		const
		struct chess_position curr_top_right_pos = { top_right_row, top_right_col };

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_bishop_possibilities: Considering top right possibility: (%d, %d)\n",
		        curr_top_right_pos.row, curr_top_right_pos.col);
#endif

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_top_right_pos))
		{
			break;
		}
	}

	// search for possibilities in bottom left diagonal
	for (square_index_type bottom_left_row = coin_to_update->position.row + 1,
	                       bottom_left_col = coin_to_update->position.col - 1;
	     bottom_left_row < game->board_limits.row && bottom_left_col >= 0;
	     bottom_left_row++, bottom_left_col--)
	{
		const
		struct chess_position curr_bottom_left_pos = { bottom_left_row, bottom_left_col };

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_bishop_possibilities: Considering bottom left possibility: (%d, %d)\n",
		        curr_bottom_left_pos.row, curr_bottom_left_pos.col);
#endif

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_bottom_left_pos))
		{
			break;
		}
	}

	// search for possibilities in bottom right diagonal
	for (square_index_type bottom_right_row = coin_to_update->position.row + 1,
	                       bottom_right_col = coin_to_update->position.col + 1;
	     bottom_right_row < game->board_limits.row && bottom_right_col < game->board_limits.col;
	     bottom_right_row++, bottom_right_col++)
	{
		const
		struct chess_position curr_bottom_right_pos = { bottom_right_row, bottom_right_col };

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_bishop_possibilities: Considering bottom right possibility: (%d, %d)\n",
		        curr_bottom_right_pos.row, curr_bottom_right_pos.col);
#endif

		if (update_rook_bishop_possibilities_helper (game, coin_to_update, curr_bottom_right_pos))
		{
			break;
		}
	}
}

/**
 * update_queen_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Updates the possible positions to which the coin in the given position could move
 * considering it to be a queen.
 *
 * Note:
 * 1. A valid coin is expected to be found in the given position.
 * 2. The possibility vector must be empty i.e., the previous possibilities if any should be truncated.
 */
static
void update_queen_possibilities (const struct chess *const game,
                                 const square_index_type curr_coin_row,
                                 const square_index_type curr_coin_col)
{
	// Take advantage of the fact that the queen combines the powers of the rook and the bishop
	update_rook_possibilities (game, curr_coin_row, curr_coin_col);
	update_bishop_possibilities (game, curr_coin_row, curr_coin_col);
}

int update_possibilities (const struct chess *const game,
                          const square_index_type curr_coin_row,
                          const square_index_type curr_coin_col)
{
	struct chess_coin *const coin_to_update = *(*(game->board + curr_coin_row) + curr_coin_col);

	if (coin_to_update == NULL)
	{
		return 1;
	}

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_possibilities: Got request to update possibilities for coin at (%d, %d)\n",
	        curr_coin_row, curr_coin_col);
#endif

	// initially free the old possibilities
	free (coin_to_update->possible_moves);
	coin_to_update->possible_moves = NULL;
	coin_to_update->possible_moves_num = 0;

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
	printf ("update_possibilities: Successfully cleared previous possibilities.\n");
#endif

	switch (coin_to_update->type)
	{
	case PAWN:

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_possibilities: the coin is a 'PAWN'\n");
#endif

		update_pawn_possibilities (game, curr_coin_row, curr_coin_col);
		break;

	case KING:

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_possibilities: the coin is a 'KING'\n");
#endif

		update_king_possibilities (game, curr_coin_row, curr_coin_col);
		break;

	case KNIGHT:

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_possibilities: the coin is a 'KNIGHT'\n");
#endif

		update_knight_possibilities (game, curr_coin_row, curr_coin_col);
		break;

	case ROOK:

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_possibilities: the coin is a 'ROOK'\n");
#endif

		update_rook_possibilities (game, curr_coin_row, curr_coin_col);
		break;

	case BISHOP:

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_possibilities: the coin is a 'BISHOP'\n");
#endif

		update_bishop_possibilities (game, curr_coin_row, curr_coin_col);
		break;

	case QUEEN:

#ifdef KS_CHESS_UPDATE_POSSIBILITIES_DEBUG
		printf ("update_possibilities: the coin is a 'QUEEN'\n");
#endif

		update_queen_possibilities (game, curr_coin_row, curr_coin_col);
		break;
	}

	return 0;
}
