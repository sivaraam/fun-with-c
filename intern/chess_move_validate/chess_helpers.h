#include "chess.h"

/**
 * initialise_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Initialise the possibilities for the coin in the given position.
 * Initialisation refers to initialising the possible moves of a coin in a new board.
 *
 * Returns: 0 in case of success or a non-zero value in case of failure.
 */
int initialise_possibilities (struct chess *const game,
                             square_index_type curr_coin_row,
                             square_index_type curr_coin_col);

/**
 * update_possibilities:
 *
 * @game: The structure that represents the current game state which includes the
 *        positions of different coins on the board
 * @curr_coin_row: The index of the row in which the coin resides
 * @curr_coin_col: The index of the column in which the coin resides
 *
 * Update the possibilities of the coin in the given position for the given game state.
 *
 * Returns: 0 on success or a non-zero value in case of failure.
 */
int update_possibilities (const struct chess *const game,
                          const square_index_type curr_coin_row,
                          const square_index_type curr_coin_col);
