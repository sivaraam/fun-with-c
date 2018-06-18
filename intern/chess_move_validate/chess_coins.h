#ifndef KS_CHESS_COINS
#define KS_CHESS_COINS

#include <stddef.h>

#define ERRNULL 1
#define ERRMEMORY  2

typedef char square_index_type;

/**
 * Coin type constants
 */
#define PAWN 'p'
#define KNIGHT 'n'
#define ROOK 'r'
#define BISHOP 'b'
#define QUEEN 'q'
#define KING 'k'

/**
 * Chess coin colour constants
 */
#define WHITE_COIN 1
#define BLACK_COIN -1

struct chess_position
{
	square_index_type row, col;
};

/**
 * The structure that represents a valid chess coin
 * in a chess board.
 *
 * @type: Type of chess coin.
 *        One of:
 *
 *           - ROOK
 *           - KNIGHT
 *           - BISHOP
 *           - QUEEN
 *           - KING
 *           - PAWN
 *
 * @colour: Indicates whether the colour of the coin
 *
 * @position: Position of the coin in the chess board.
 *            The array should be indexed using POSITION_ROW, POSITION_COL.
 *            The values are in the range [0, 7].
 *
 * @possible_moves: An array of possible positions that the coin at the given
 *                  position can move to if the coin was about to be moved next.
 *
 * Note: The values of most members in this structure make sense only when used
 *       alongside a valid 'struct chess' object.
 *
 */
struct chess_coin
{
	char type;
	_Bool is_pawns_first_move;
	int colour;
	struct chess_position position;
	struct chess_position *possible_moves;
	unsigned possible_moves_num;
};

/** Coin default values **/

/* White coins */
extern const struct chess_coin WHITE_ROOK;
extern const struct chess_coin WHITE_KNIGHT;
extern const struct chess_coin WHITE_BISHOP;
extern const struct chess_coin WHITE_QUEEN;
extern const struct chess_coin WHITE_KING;
extern const struct chess_coin WHITE_PAWN;

/* Black coins */
extern const struct chess_coin BLACK_ROOK;
extern const struct chess_coin BLACK_KNIGHT;
extern const struct chess_coin BLACK_BISHOP;
extern const struct chess_coin BLACK_QUEEN;
extern const struct chess_coin BLACK_KING;
extern const struct chess_coin BLACK_PAWN;

/**
 * chess_coin_init_helper:
 *
 * @row: The row in which the coin to initialise is present
 * @col: The column in which the coin to initalise is present
 * @coin_to_init: The coin to initialise
 * @default_val: The default values for the coin
 *
 * Helper function to initialise the members of the coin to their defaults
 */
int chess_coin_init_helper (square_index_type row, square_index_type col,
                            struct chess_coin **const coin_to_init, const struct chess_coin default_val);

/**
 * insert_possible_move:
 *
 * @coin: Coin for which (row, col) should be inserted as a possible move
 * (@row, @col): The dimensions whihc represent a possible move for the given coin
 *
 * Helper function to insert (@row, @col) as a possible move for @coin.
 *
 * Returns: 0 in case of success or non-zero value indicating the error in case of failure.
 */
int insert_possible_move (struct chess_coin *const coin, square_index_type row, square_index_type col);

#endif
