/**
 * A simple test application that creates an
 * window that displays images and listens to
 * certain evnets realted to images.
 */

#include <math.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static
void getsize (GtkWidget *widget,
              GtkAllocation *allocation,
              gpointer data)
{
	g_print ("grid: width: %d; height: %d\n", allocation->width, allocation->height);
}

static
gboolean motion_notify_event_cb (GtkWidget *widget,
                                 GdkEventMotion *event,
                                 GtkWidget *window)
{
	gint grid_x = 0, grid_y = 0;
	int round_x = round (event->x), round_y = round (event->y);

	g_print ("Got x: %d, y: %d\n", round_x, round_y);

	gtk_widget_translate_coordinates (GTK_WIDGET(widget), window,
	                                  round (event->x), round (event->y), &grid_x, &grid_y);

	g_print ("grid_event_box: translated coordiantes: x: %d, y: %d\n", grid_x, grid_y);

	return FALSE;
}

static
void activate(GtkApplication *app,
              gpointer user_data)
{
	GtkWidget *window = NULL;
	GtkWidget *grid = NULL;
	GtkWidget *grid_event_box = NULL;
	GtkWidget *image = NULL;
	GtkWidget *image_1 = NULL;
	GdkPixbuf *image_buf = NULL;
	GdkPixbuf *image_buf_copy = NULL;
	GError *image_buf_load_error = NULL;
	static const char *image_path = "../../files/maze/test_inputs.bak/BMP1.bmp";

	/* Create the window */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Image window");
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	/* Create the grid to hold multiple images */
	grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);

	/* Create the GdkPixbuf object for the images that are to be loaded */
	image_buf = gdk_pixbuf_new_from_file (image_path, &image_buf_load_error);

	g_assert ((image_buf == NULL && image_buf_load_error != NULL) ||
	          (image_buf != NULL && image_buf_load_error == NULL));

	/* The copy is not necessary. This is just to try reference handling. */
	image_buf_copy = g_object_ref (image_buf);

	/* Attach images to the grid if successful */
	if (image_buf_load_error == NULL)
	{
		g_assert (image_buf != NULL);

		image = gtk_image_new_from_pixbuf (image_buf);
		image_1 = gtk_image_new_from_pixbuf (image_buf_copy);

		gtk_grid_attach (GTK_GRID (grid), image, 0, 0, 1, 1);
		gtk_grid_attach (GTK_GRID (grid), image_1, 1, 0, 1, 1);

		g_assert (image_1 == gtk_grid_get_child_at (GTK_GRID (grid), 1, 0));

		g_signal_connect (grid, "size-allocate", G_CALLBACK (getsize), NULL);

		/* Attach an event box to the grid to receive events */
		grid_event_box = gtk_event_box_new ();
		gtk_container_add (GTK_CONTAINER (grid_event_box), grid);

		/* Attach the event box to the window */
		gtk_container_add (GTK_CONTAINER (window), grid_event_box);

		g_signal_connect (grid_event_box, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), window);

		gtk_widget_set_events (grid_event_box, gtk_widget_get_events (grid_event_box)
	                                     | GDK_POINTER_MOTION_MASK);
	}
	else
	{
		// Report error to user, and free error
		g_assert (image_buf == NULL);

		g_print("Loading image failed with error : %s\n", image_buf_load_error->message);
		g_error_free (image_buf_load_error);
	}

	g_object_unref (image_buf_copy);
	gtk_widget_show_all (window);
}

int main(int argc, char *argv[])
{
	GtkApplication *app = NULL;
	int status = 0;

	app = gtk_application_new ("com.ks.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
