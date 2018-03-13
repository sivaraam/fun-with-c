/**
 * A simple GUI application that allows the
 * user to interact with a chess board that
 * does simple move validation.
 */

#include <math.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "../../chess_move_validate/chess.h"
#include "square-paths.h"

/**
 * Grid cell constants
 */
static const gint grid_cell_side = 50;
static const gint grid_cells_horizontal = 1;
static const gint grid_cells_vertical = 1;

// Data related to game
struct chess *game = NULL;

/**
 * get_square_type_index
 *
 * @square_type: A character that indicates the type of square.
 *
 * Returns the index to be used for the 'square_type' dimension of
 * 'square_image_paths'.
 *
 * All characters other than those representing valid chess coins
 * are considered to be empty square.
 *
 * Returns: A valid 'enum square_type_indices' value.
 */
inline static
enum square_type_indices get_square_type_index (const char square_type)
{
	switch (square_type)
	{
	case PAWN:
		return PAWN_INDEX;

	case KNIGHT:
		return KNIGHT_INDEX;

	case ROOK:
		return ROOK_INDEX;

	case BISHOP:
		return BISHOP_INDEX;

	case QUEEN:
		return QUEEN_INDEX;

	case KING:
		return KING_INDEX;

	default:
		return EMPTY_INDEX;
	}
}

/**
 * get_coin_colour_index:
 *
 * @coin_colour: Value representing the colour of a valid chess coin.
 *
 * Returns the index to be used for the 'coin colour' dimension of
 * 'square_image_paths'.
 *
 * Returns: A valid 'enum coin_colour_indices' value.
 */
inline static
enum coin_colour_indices get_coin_colour_index (const int coin_colour)
{
	switch (coin_colour)
	{
	case BLACK_COIN:
		return BLACK;

	case WHITE_COIN:
		return WHITE;

	default:
		g_assert(FALSE); /* BUG: this shouldn't happen */
	}

	/* for sanity */
	return 0;
}

/**
 * get_square_colour_index:
 *
 * (@row, @col): The coordinates of the square whose background colour must
 *               be identified.
 *
 * Returns the index to be used for the 'square colour' dimension of
 * 'square_image_paths'.
 *
 * Returns: A valid 'enum square_colour_indices' value.
 *
 */
inline static
enum square_colour_indices get_square_colour_index (const square_index_type row, const square_index_type col)
{
/*	if (col & 1)
	{
		if (row & 1)
		{
			return DARK;
		}
		else
		{
			return LIGHT;
		}
	}
	else
	{
		if (row & 1)
		{
			return DARK;
		}
		else
		{
			return LIGHT;
		}
	}*/

	return !((row + col) & 1) ? DARK : LIGHT;
}

/**
 * get_grid_left_top:
 *
 * (@x, @y): The grid window relative coordinates.
 * (@left, @top): The values used to identify the child corresponding
 *                to the given coordinates.
 *
 * Tranlsate the grid window relative (x, y) coordinates,
 * into (left, top) values values which can be used to
 * index the widget found under (x, y) in the grid.
 *
 * Assumes that each widget are squares and have a constant
 * 'length' in the sides which is pre-determined.
 */
static
void get_grid_left_top(const gint x, const gint y,
                       gint *const left, gint *const top)
{
	*left = x/grid_cell_side;
	*top = y/grid_cell_side;
}

/**
 * get_pixbuf_for_pos:
 *
 * (@row, @col): The coordinates that identify the position of the square in
 *               the game board.
 * @square_type: A character that identifies the type of coin in the square
 *               or an empty square.
 * @coin_colour: A value that specifies the colour of the coin in the square.
 *               The value can be either 'BLACK' or 'WHITE' for an empty cells.
 *
 * Returns a valid pixbuf for the corresponding image of the square (with/without
 * a coin) for the given position. The image is scaled to the required 'width'
 * and 'height'.
 *
 * Returns: A pointer to a valid Pixbuf on success or NULL in case of an error.
 *
 * Note: The reference handling for the returned Pixbuf lies in the hands of the caller.
 */
static
GdkPixbuf *get_pixbuf_for_pos (const square_index_type row,
                               const square_index_type col,
                               const char square_type,
                               const int coin_colour)
{
	static const gint required_width = 50, required_height = 50;

	const enum square_type_indices st_index = get_square_type_index (square_type);
	const enum coin_colour_indices cc_index = get_coin_colour_index (coin_colour);
	const enum square_colour_indices sc_index = get_square_colour_index (row, col);

	/* Get the path to the image with given characteristics */
	const char *curr_coin_path = square_image_paths [st_index][cc_index][sc_index];

	GdkPixbuf *coin_image_buf;
	GdkPixbuf *scaled_coin_image_buf;
	GError *coin_image_buf_load_error = NULL;

	coin_image_buf = gdk_pixbuf_new_from_file (curr_coin_path, &coin_image_buf_load_error);

	g_assert ((coin_image_buf == NULL && coin_image_buf_load_error != NULL) ||
	          (coin_image_buf != NULL && coin_image_buf_load_error == NULL));

	if (coin_image_buf_load_error == NULL)
	{
		/* Scale the image to the required dimensions */
		scaled_coin_image_buf = gdk_pixbuf_scale_simple (coin_image_buf,
		                                                 required_width, required_height,
		                                                 GDK_INTERP_BILINEAR);

		g_assert (scaled_coin_image_buf != NULL);

		/*
		 * Unref the image buf as its not required anymore.
		 * The scaled image is a copy and doesn't depend on this.
		 */
		g_object_unref (coin_image_buf);

		return scaled_coin_image_buf;
	}
	else
	{
		g_print("Loading coin image failed with error : %s\n", coin_image_buf_load_error->message);
		g_error_free (coin_image_buf_load_error);
		return NULL;
	}
}

/**
 * update_coin_at_pos:
 *
 * @grid: The grid representing the chess board.
 *
 * Replace the image in the grid cell corresponding to the destination
 * with the image in the grid cell corresponding to the source.
 *
 * Fill in the grid at the destination with the image of an empty cell.
 */
static
void update_coin_at_pos (GtkGrid *const grid,
                         const gint source_left, const gint source_top,
                         const gint dest_left, const gint dest_top)
{
	GtkWidget *image_at_dest;
	GtkWidget *image_at_source;
	GdkPixbuf *new_source_image_buf;
	GdkPixbuf *new_dest_image_buf;

	/*
	 * The coordinates to be used to access the coins in the board.
	 * The (x,y) coordinates always correspond to (top, left).
	 */
	const square_index_type source_x = source_top, source_y = source_left,
	                        dest_x = dest_top, dest_y = dest_left;

	/* The source coin before moving is now the new destination coin */
	const struct chess_coin *const new_dest_coin = *(*(game->board + dest_x) + dest_y);

	g_assert (new_dest_coin != NULL);

	image_at_source = gtk_grid_get_child_at (grid, source_left, source_top);
	image_at_dest = gtk_grid_get_child_at (grid, dest_left, dest_top);

	g_assert (image_at_dest != NULL && image_at_source != NULL);

	new_dest_image_buf = get_pixbuf_for_pos (dest_x, dest_y, new_dest_coin->type, new_dest_coin->colour);

	/*
	 * Empty square replacement for the source.
	 */
	new_source_image_buf = get_pixbuf_for_pos (source_x, source_y, 0, BLACK_COIN);

	g_assert (new_source_image_buf != NULL && new_dest_image_buf);

	if (new_source_image_buf == NULL || new_dest_image_buf == NULL)
	{
		g_print ("Updating image position failed!\n");
		g_print ("source: row: %d, col: %d\n", source_top, source_left);
		g_print ("dest: row: %d, col: %d\n", dest_top, dest_left);
		return;
	}

	gtk_image_set_from_pixbuf (GTK_IMAGE (image_at_dest), new_dest_image_buf);
	gtk_image_set_from_pixbuf (GTK_IMAGE (image_at_source), new_source_image_buf);

	g_object_unref (new_source_image_buf);
	g_object_unref (new_dest_image_buf);
}

/**
 * drag_begin_cb:
 *
 * @drag: The object containing the information related to the current drag.
 * (@offset_x, @offset_y): The coordinates of the position at which the drag begun
 *                         relative to the event window.
 * @user_data: Any data passed to the callback. (Not used currently)
 *
 * The callback function that is called when a drag gesture begins.
 */
static
void drag_begin_cb (const GtkGestureDrag *const drag,
                    const gdouble offset_x,
                    const gdouble offset_y,
                    const gpointer user_data)
{
	gint drag_start_x = round (offset_x), drag_start_y = round (offset_y);

	g_print ("drag_begin_cb: Start point: Got        x: %lf\t y:%lf\n", offset_x, offset_y);
	g_print ("drag_begin_cb: Start point: Rounded to x: %d\ty: %d\n", drag_start_x, drag_start_y);
}

/**
 * drag_end_cb:
 *
 * @drag: The object containing the information related to the current drag.
 * (@offset_x, @offset_y): The coordinates of the position at which the drag ended
 *                         relative to the event window.
 * @grid: The grid representing the chess board.
 *
 * The callback function that is called when a drag gesture ends.
 */
static
void drag_end_cb (GtkGestureDrag *const drag,
                  const gdouble offset_x,
                  const gdouble offset_y,
                  GtkGrid *const grid)
{
	gint drag_end_offset_x = round (offset_x), drag_end_offset_y = round (offset_y);
	gdouble d_drag_start_x = 0.0, d_drag_start_y = 0.0;

	g_print ("drag_end_cb: End point: Got        x: %lf\t y: %lf\n", offset_x, offset_y);
	g_print ("drag_end_cb: End point: Rounded to x: %d\t y: %d\n", drag_end_offset_x, drag_end_offset_y);

	if (gtk_gesture_drag_get_start_point (drag, &d_drag_start_x, &d_drag_start_y))
	{
		gint drag_end_x= 0, drag_end_y = 0,
		     drag_start_x = 0, drag_start_y = 0;

		/* Coordinates to manipulate images in the grid */
		gint source_left = 0, source_top = 0,
		     dest_left = 0, dest_top = 0;

		/* Coordinates to identify squares in the game board */
		gint source_x = 0, source_y = 0,
		     dest_x = 0, dest_y = 0;

		drag_start_x = round (d_drag_start_x), drag_start_y = round (d_drag_start_y);

		g_print ("drag_end_cb: Start point: Got        x: %lf\t y:%lf\n", d_drag_start_x, d_drag_start_y);
		g_print ("drag_end_cb: Start point: Rounded to x: %d\ty: %d\n", drag_start_x, drag_start_y);

		drag_end_x = drag_start_x + drag_end_offset_x;
		drag_end_y = drag_start_y + drag_end_offset_y;
		g_print ("drag_end_cb: End point: Actual x: %d, y: %d\n", drag_end_x,
		                                                          drag_end_y);

		get_grid_left_top (drag_start_x, drag_start_y, &source_left, &source_top);
		get_grid_left_top (drag_end_x, drag_end_y, &dest_left, &dest_top);

		/* The (x,y) coordinates of the game board always correspond to (top, left) */
		source_x = source_top, source_y = source_left;
		dest_x = dest_top, dest_y = dest_left;

		g_print ("drag_end_cb: source_left: %d, source_top: %d\n",
		         source_left, source_top);
		g_print ("drag_end_cb: dest_left: %d, dest_top: %d\n",
		         dest_left, dest_top);

		/*
		 * Take action only if the coordinates are whithin the board limits
		 * and the source coordinate correpsonds to a valid coin (not empty).
		 */
		if (source_x < game->board_limits.row &&
		    source_y < game->board_limits.col &&
		    dest_x < game->board_limits.row &&
		    dest_y < game->board_limits.col
		   )
		{
			/*
			 * Do nothing the source and dest are the same (or)
			 * when there is no coin at the source.
			 */
			if (
			    (source_left == dest_left &&
			     source_top == dest_top) ||
			     *(*(game->board + source_x) + source_y) == NULL
			   )
			{
				return;
			}

			const int move_status = move_coin (game, source_top, source_left, dest_top, dest_left);

			g_assert (move_status != ERR_NULL);

			if (move_status == ERR_TURN)
			{
				g_print ("It's not your turn!\n");
				return;
			}

			switch (move_status)
			{
			case MOVE_ILLEGAL:
				g_print ("Illegal move!\n");
				return;

			case MOVE_OCCUPY:
				g_print ("Coin at source \"occupied\" the destination position\n");
				break;

			case MOVE_TAKEDOWN:
				g_print ("Coin at source \"took down\" the coin at the destination\n");
				break;
			}

			/* Update the display state */
			update_coin_at_pos (grid, source_left, source_top, dest_left, dest_top);
		}
	}
	else
	{
		g_print ("drag_end_cb: Could not get start point!\n");
	}
}

/**
 * initialise_coin_at_pos:
 *
 * @grid: The grid representing the chess board.
 * (@row, @col): The coordinates corresponding to the square of the game board
 *               to initialise.
 * @square_type: A character that identifies the type of coin in the square
 *               or an empty square.
 * @coin_colour: A value that specifies the colour of the coin in the square.
 *
 * The function that initialises the image for the cell in the graphical board corresponding
 * to the given game board cell.
 *
 * Returns: 0 in case of success or a non-negative value in case of an error.
 */
static
int initialise_coin_at_pos (GtkGrid *const grid,
                        const square_index_type row,
                        const square_index_type col,
                        const char square_type,
                        const int coin_colour)
{
	GtkWidget *coin_image;
	GdkPixbuf *coin_image_buf;

	/* The (top, left) coordinates of the game board always correspond to (row, col) */
	const gint coin_grid_left = col, coin_grid_top = row;

	coin_image_buf = get_pixbuf_for_pos (row, col, square_type, coin_colour);

        /* Attach image to the grid if successful */
	if (coin_image_buf != NULL)
	{
		coin_image = gtk_image_new_from_pixbuf (coin_image_buf);

		gtk_grid_attach (GTK_GRID (grid), coin_image, coin_grid_left, coin_grid_top,
		                 grid_cells_horizontal, grid_cells_vertical);
	}
	else
	{
		g_print ("Initialising coin with coin image failed!\n");
		return 1;
	}

	/* Unref the pixbuf buffer */
	g_object_unref (coin_image_buf);
	return 0;
}

/**
 * initialise_board_grid:
 *
 * @grid: The grid representing the chess board that needs to be initialised.
 *
 * Initialises the visual board grid with images corresponding to the coins
 * in the game board.
 *
 * Returns: 0 in case of success or non-zero value in case of an error.
 */
static
int initialise_board_grid (GtkGrid *board_grid)
{
	g_assert (board_grid != NULL && game != NULL);

	for (square_index_type row = 0; row < game->board_limits.row; row++)
	{
		for (square_index_type col = 0; col < game->board_limits.col; col++)
		{
			const struct chess_coin *const curr_coin = *(*(game->board + row) + col);
			const char square_type = (curr_coin != NULL) ? curr_coin->type : 0;
			const int coin_colour = (curr_coin != NULL) ? curr_coin->colour : BLACK_COIN;

			if (initialise_coin_at_pos (board_grid, row, col, square_type, coin_colour))
			{
				g_print ("Updating coin position failed!\n");
				return 1;
			}
		}
	}

	return 0;
}

/**
 * activate:
 *
 * @app: Pointer to the main app instance.
 * @user_Data: Data passed to the activation function (Currently unused)
 *
 * Function that sets up the application by initialising the required data
 * and setting the user interface by registering for the required events etc.
 */
static
void activate(GtkApplication *app,
              gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *board_grid;
	GtkWidget *grid_event_box;
	GtkGesture *grid_drag_gesture;

	/* Get a new game instance */
	game = get_new_game();

	/* Create the window */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Chess move validation");
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

	board_grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous (GTK_GRID (board_grid), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID (board_grid), TRUE);

	if (initialise_board_grid(GTK_GRID (board_grid)))
	{
		return;
	}


	/* Attach an event box to the grid to receive events */
	grid_event_box = gtk_event_box_new ();
	gtk_container_add (GTK_CONTAINER (grid_event_box), board_grid);

	/* Attach the event box to the window */
	gtk_container_add (GTK_CONTAINER (window), grid_event_box);

	/* Attach a drag gesture to the event box */
	grid_drag_gesture = gtk_gesture_drag_new (grid_event_box);
	g_signal_connect (grid_drag_gesture, "drag-begin", G_CALLBACK (drag_begin_cb), NULL);
	g_signal_connect (grid_drag_gesture, "drag-end", G_CALLBACK (drag_end_cb), GTK_GRID (board_grid));

	gtk_widget_set_events (grid_event_box, gtk_widget_get_events (grid_event_box)
                                     | GDK_POINTER_MOTION_MASK);

	gtk_widget_show_all (window);
}

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status = 0;

	app = gtk_application_new ("com.ks.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
