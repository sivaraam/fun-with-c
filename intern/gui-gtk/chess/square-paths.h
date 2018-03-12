#ifndef KS_CHESS_GUI_SQAURE_PATHS
#define KS_CHESS_GUI_SQAURE_PATHS

/**
 * The three-dimensional array of character strings that
 * contains the path to images required for the chess board.
 *
 * Each image has three different properties:
 *
 *     - Square type (a valid coin type or EMPTY)
 *     - Coin colour (the colour of a valid coin)
 *     - Square colour (the colour of the background over which
 *                      the coin is placed)
 *
 * Each dimension of the array corresponds to a category.
 */
extern const char *square_image_paths[7][2][2];

/**
 * The constants that identify the 'square type' index
 * to be used for the different coins and the empty square
 * in the 'square type' dimension of 'square_image_paths'.
 */
enum square_type_indices
{
    BISHOP_INDEX,
    KING_INDEX,
    KNIGHT_INDEX,
    PAWN_INDEX,
    QUEEN_INDEX,
    ROOK_INDEX,
    EMPTY_INDEX
};

/**
 * The constants that identify the index to be used for different
 * colours of coins in the 'coin colour' dimension
 * of 'square_image_paths'.
 */
enum coin_colour_indices
{
	BLACK,
	WHITE
};

/**
 * The constants that identify the index to be used for different square
 * colours in the 'square colour' dimension of 'square_image_paths'.
 */
enum square_colour_indices
{
	DARK,
	LIGHT
};

#endif
