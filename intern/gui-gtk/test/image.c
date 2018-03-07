/**
 * A simple test application that creates an
 * window that display an image.
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>


static
void activate(GtkApplication *app,
              gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *image;
	GdkPixbuf *image_buf;
	GError *image_buf_load_error = NULL;
	static const char *image_path = "../../files/maze/test_inputs.bak/BMP1.bmp";

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Image window");
	gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

	image_buf = gdk_pixbuf_new_from_file (image_path, &image_buf_load_error);

	g_assert ((image_buf == NULL && image_buf_load_error != NULL) ||
	          (image_buf != NULL && image_buf_load_error == NULL));

	if (image_buf_load_error == NULL)
	{
		g_assert (image_buf != NULL);
		image = gtk_image_new_from_pixbuf (image_buf);
		gtk_container_add (GTK_CONTAINER (window), image);
	}
	else
	{
		// Report error to user, and free error
		g_assert (image_buf == NULL);
		g_print("Loading image failed with error : %s\n", image_buf_load_error->message);
		g_error_free (image_buf_load_error);
	}

	gtk_widget_show_all (window);
}

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("com.ks.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
