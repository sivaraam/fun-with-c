#ifndef KS_CHESS
#define KS_CHESS

#include "chess_coins.h"

#define ERR_NULL -1
#define ERR_TURN -128

/**
 * The structure that represents a chess game.
 *
 * @board_limits: Indicate the row and column limits of the game board.
 *
 * @board: The current state of the board as pointers to 'struct chess_coin'
 *         objects. A 'NULL' value represents that the position square is
 *         unoccupied.
 *
 * @is_whites_turn: Indicates whether the current is that of the white player.
 *
 */
struct chess
{
	struct chess_position board_limits;
	struct chess_coin ***board;
	_Bool is_whites_turn;
};

/**
 * get_new_game:
 *
 * Returns a new instance of 'struct chess' that could be used for a new game.
 * Thus the board will be initialised with all coins positioned as required in
 * the start of the game.
 *
 * Returns: a pointer to a valid 'struct chess' instance on success or NULL in
 *          case of failure.
 */
struct chess *get_new_game(void);

enum move_type
{
	MOVE_ILLEGAL,
	MOVE_OCCUPY,
	MOVE_TAKEDOWN
};

/**
 * move_coin:
 *
 * @game: The structure that represents the current game state
 * (@src_row, @src_col): The coordinates of the position of the source coin
 *                       in the game board
 * (@dest_row, @dest_col): The coordinates of the position of the destination
 *                         to which the coin must be moved
 *
 * Move a coin found in the source coordinates to the position idenitified by the
 * destination coordinates by validating whether it is a valid move for the coin
 * at source.
 *
 * Note: The source position should contain a valid coin. It is an error if it doesn't.
 *
 * Returns: A non-negative value indicating the type of move. Negative value in case
 *          of an error.
 */
int move_coin (struct chess *game,
               square_index_type src_row,
               square_index_type src_col,
               square_index_type dest_row,
               square_index_type dest_col);

#endif
