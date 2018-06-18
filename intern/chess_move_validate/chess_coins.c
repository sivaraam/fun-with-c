#include <stdlib.h>
#include "chess_coins.h"

/** Coin default values **/

/* White coins */
const struct chess_coin WHITE_ROOK    = { ROOK,   0, 1, {0}, NULL, 0 };
const struct chess_coin WHITE_KNIGHT  = { KNIGHT, 0, 1, {0}, NULL, 0 };
const struct chess_coin WHITE_BISHOP  = { BISHOP, 0, 1, {0}, NULL, 0 };
const struct chess_coin WHITE_QUEEN   = { QUEEN,  0, 1, {0}, NULL, 0 };
const struct chess_coin WHITE_KING    = { KING,   0, 1, {0}, NULL, 0 };
const struct chess_coin WHITE_PAWN    = { PAWN,   1, 1, {0}, NULL, 0 };

/* Black coins */
const struct chess_coin BLACK_ROOK    = { ROOK,   0, -1, {0}, NULL, 0 };
const struct chess_coin BLACK_KNIGHT  = { KNIGHT, 0, -1, {0}, NULL, 0 };
const struct chess_coin BLACK_BISHOP  = { BISHOP, 0, -1, {0}, NULL, 0 };
const struct chess_coin BLACK_QUEEN   = { QUEEN,  0, -1, {0}, NULL, 0 };
const struct chess_coin BLACK_KING    = { KING,   0, -1, {0}, NULL, 0 };
const struct chess_coin BLACK_PAWN    = { PAWN,   1, -1, {0}, NULL, 0 };

int chess_coin_init_helper (square_index_type row, square_index_type col,
                            struct chess_coin **coin_to_init, struct chess_coin default_val)
{
	if (coin_to_init == NULL)
	{
		return 1;
	}

	*coin_to_init = malloc (sizeof (struct chess_coin));

	if (*coin_to_init == NULL)
	{
		return 1;
	}

	/* Initially initialise coin with the default value for the coin */
	**coin_to_init = default_val;

	/* Initialise the position */
	(*coin_to_init)->position.row = row;
	(*coin_to_init)->position.col = col;

	return 0;
}

/**
 * get_increment:
 *
 * @coin_type: Character representing the type of chess coin
 *
 * Helper function that returns the increments by which units of memory
 * must be allocated for storing the possible moves of a coin of the
 * given type (@coin_type).
 *
 * Generally, it is half the amount of maximum possible moves of the given coin
 * type rounded to the nearest even integer.
 *
 * Returns: The memory increment unit for the given coin type.
 */
static
unsigned get_increment (char coin_type)
{
	switch (coin_type)
	{
	case PAWN:
		return 2;

	case KING:
		return 4;

	case KNIGHT:
		return 4;

	case ROOK:
		return 7;

	case BISHOP:
		return 7;

	case QUEEN:
		return 14;

	default:
		return 0;
	}
}

int insert_possible_move (struct chess_coin *const coin,
                          const square_index_type row,
                          const square_index_type col)
{
	if (coin == NULL)
	{
		return ERRNULL;
	}

	const unsigned increment = get_increment (coin->type);
	const struct chess_position possible_move_pos = { row, col };

	// re-allocate when necessary
	if (coin->possible_moves_num % increment == 0)
	{
		struct chess_position *temp = realloc(coin->possible_moves,
		                                      (coin->possible_moves_num + increment) * sizeof(struct chess_position));
		if (temp == NULL)
		{
			return ERRMEMORY;
		}
		coin->possible_moves = temp;
	}

	*(coin->possible_moves + coin->possible_moves_num) = possible_move_pos;
	(coin->possible_moves_num)++;

	return 0;
}
