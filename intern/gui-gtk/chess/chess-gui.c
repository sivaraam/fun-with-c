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

static
enum square_type_indices get_square_type_index (char square_type)
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

static
enum coin_colour_indices get_coin_colour_index (int coin_colour)
{
	switch (coin_colour)
	{
	case BLACK_COIN:
		return BLACK;

	case WHITE_COIN:
		return WHITE;

	default:
		g_assert(FALSE); // BUG: this shouldn't happen
	}

	// for sanity
	return 0;
}

inline static
_Bool is_dark_square (square_index_type row, square_index_type col)
{
/*	if (col & 1)
	{
		if (row & 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (row & 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}*/
	return !((row + col) & 1);
}

/**
 * Given the grid window relative (x, y) coordinates,
 * finds the corresonding (left, top) values which can
 * be used to index the widget found at (x, y) in the grid.
 *
 * Assumes that each widget are squares and have a constant
 * 'length' in the sides which is pre-determined.
 */
static
void get_grid_left_top(gint x, gint y,
                       gint *left, gint *top)
{
	*left = x/grid_cell_side;
	*top = y/grid_cell_side;
}

static
void drag_begin_cb (GtkGestureDrag *drag,
                    gdouble offset_x,
                    gdouble offset_y,
                    gpointer user_data)
{
	gint drag_start_x = round (offset_x), drag_start_y = round (offset_y);

	g_print ("drag_begin_cb: Start point: Got        x: %lf\t y:%lf\n", offset_x, offset_y);
	g_print ("drag_begin_cb: Start point: Rounded to x: %d\ty: %d\n", drag_start_x, drag_start_y);
}

static
GdkPixbuf *get_pixbuf_for_pos (square_index_type row,
                               square_index_type col,
                               const char square_type,
                               const int coin_colour)
{
	static const gint required_width = 50, required_height = 50;

	const enum square_type_indices st_index = get_square_type_index (square_type);
	const enum coin_colour_indices cc_index = get_coin_colour_index (coin_colour);
	const enum square_colour_indices sc_index = (is_dark_square (row, col)) ? DARK : LIGHT;
	const char *curr_coin_path = square_image_paths [st_index][cc_index][sc_index];

	GdkPixbuf *coin_image_buf;
	GdkPixbuf *scaled_coin_image_buf;
	GError *coin_image_buf_load_error = NULL;

	/* Create the GdkPixbuf object for the images that are to be loaded */
	coin_image_buf = gdk_pixbuf_new_from_file (curr_coin_path, &coin_image_buf_load_error);

	g_assert ((coin_image_buf == NULL && coin_image_buf_load_error != NULL) ||
	          (coin_image_buf != NULL && coin_image_buf_load_error == NULL));

	if (coin_image_buf_load_error == NULL)
	{
		scaled_coin_image_buf = gdk_pixbuf_scale_simple (coin_image_buf,
		                                                 required_width, required_height,
		                                                 GDK_INTERP_BILINEAR);

		g_assert (scaled_coin_image_buf != NULL);

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
 * Replace the image in the grid cell corresponding to the destination
 * with the image in the grid cell corresponding to the source.
 *
 * Fill in the grid at the destination with the image of an empty cell.
 */
static
void update_coin_at_pos (GtkGrid *grid,
                         gint source_left, gint source_top,
                         gint dest_left, gint dest_top)
{
	GtkWidget *image_at_dest;
	GtkWidget *image_at_start;
	GdkPixbuf *new_source_image_buf;
	GdkPixbuf *new_dest_image_buf;

	// the previous source is now the new dest coin
	const struct chess_coin *const new_dest_coin = *(*(game->board + dest_top) + dest_left);

	g_assert (new_dest_coin != NULL);

	image_at_start = gtk_grid_get_child_at (grid, source_left, source_top);
	image_at_dest = gtk_grid_get_child_at (grid, dest_left, dest_top);

	g_assert (image_at_dest != NULL && image_at_start != NULL);

	new_dest_image_buf = get_pixbuf_for_pos (dest_top, dest_left, new_dest_coin->type, new_dest_coin->colour);

	// Empty square replacement for the destination.
	// The colour doesn't matter for empty square.
	new_source_image_buf = get_pixbuf_for_pos (source_top, source_left, 0, BLACK_COIN);

	g_assert (new_source_image_buf != NULL && new_dest_image_buf);

	if (new_source_image_buf == NULL || new_dest_image_buf == NULL)
	{
		g_print ("Updating image position failed!\n");
		g_print ("source: row: %d, col: %d\n", source_top, source_left);
		g_print ("dest: row: %d, col: %d\n", dest_top, dest_left);
		return;
	}

	gtk_image_set_from_pixbuf (GTK_IMAGE (image_at_dest), new_dest_image_buf);
	gtk_image_set_from_pixbuf (GTK_IMAGE (image_at_start), new_source_image_buf);

	g_object_unref (new_source_image_buf);
	g_object_unref (new_dest_image_buf);
}

static
void drag_end_cb (GtkGestureDrag *drag,
                  gdouble offset_x,
                  gdouble offset_y,
                  GtkGrid *grid)
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

		drag_start_x = round (d_drag_start_x), drag_start_y = round (d_drag_start_y);

		g_print ("drag_end_cb: Start point: Got        x: %lf\t y:%lf\n", d_drag_start_x, d_drag_start_y);
		g_print ("drag_end_cb: Start point: Rounded to x: %d\ty: %d\n", drag_start_x, drag_start_y);

		drag_end_x = drag_start_x + drag_end_offset_x;
		drag_end_y = drag_start_y + drag_end_offset_y;
		g_print ("drag_end_cb: End point: Actual x: %d, y: %d\n", drag_end_x,
		                                                          drag_end_y);

		get_grid_left_top (drag_start_x, drag_start_y, &source_left, &source_top);
		get_grid_left_top (drag_end_x, drag_end_y, &dest_left, &dest_top);

		g_print ("drag_end_cb: source_left: %d, source_top: %d\n",
		         source_left, source_top);
		g_print ("drag_end_cb: dest_left: %d, dest_top: %d\n",
		         dest_left, dest_top);

		/* Do nothing the source and dest are the same */
		if (source_left == dest_left &&
		    source_top == dest_top)
		{
			return;
		}

		if (source_left < game->board_limits.col &&
		    source_top < game->board_limits.row &&
		    dest_top < game->board_limits.row &&
		    dest_left < game->board_limits.col &&
		    *(*(game->board + source_top) + source_left) != NULL
		   )
		{
			int move_status = move_coin (game, source_top, source_left, dest_top, dest_left);

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

			update_coin_at_pos (grid, source_left, source_top, dest_left, dest_top);
		}
	}
	else
	{
		g_print ("drag_end_cb: Could not get start point!\n");
	}
}

static
int initialise_coin_at_pos (GtkGrid *grid,
                        square_index_type row,
                        square_index_type col,
                        const char square_type,
                        const int coin_colour)
{
	GtkWidget *coin_image;
	GdkPixbuf *coin_image_buf;

	coin_image_buf = get_pixbuf_for_pos (row, col, square_type, coin_colour);

        /* Attach image to the grid if successful */
	if (coin_image_buf != NULL)
	{
		coin_image = gtk_image_new_from_pixbuf (coin_image_buf);

		gtk_grid_attach (GTK_GRID (grid), coin_image, col, row, grid_cells_horizontal, grid_cells_vertical);
	}
	else
	{
		g_print ("Initialising coin with coin image failed!\n");
		return 1;
	}

	// unref the pixbuf buffer
	g_object_unref (coin_image_buf);
	return 0;
}

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
			const int coin_colour = (curr_coin != NULL) ? curr_coin->colour : BLACK_COIN; // the colour doesn't matter for an empty square

			if (initialise_coin_at_pos (board_grid, row, col, square_type, coin_colour))
			{
				g_print ("Updating coin position failed!\n");
				return 1;
			}
		}
	}

	return 0;
}

static
void activate(GtkApplication *app,
              gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *board_grid;
	GtkWidget *grid_event_box;
	GtkGesture *grid_drag_gesture;

	// Get a new game instance
	game = get_new_game();

	/* Create the window */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Image window");
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
