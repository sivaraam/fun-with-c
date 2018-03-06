/**
 * A simple test application that swaps the window title
 * with the label text when any key is pressed.
 */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// #define DEBUG

/**
 * Swaps the window's title with the text of the given label
 */
static
void swap_wtitle_and_ltext(GtkWidget *widget,
                           GdkEvent  *event,
                           gpointer user_data)
{
	GtkWindow *window = GTK_WINDOW (widget);
	GtkLabel *label = GTK_LABEL (user_data);

#ifdef DEBUG
	g_print ("window: %p, label: %p\n", (void *) window, (void *) label);
#endif

	if (window == NULL || label == NULL)
	{
		g_print ("Invalid arguments.\n");
		return;
	}

	char *wtitle = strdup (gtk_window_get_title (window));
	char *ltext = strdup (gtk_label_get_text (label));

#ifdef DEBUG
	g_print ("Window title: %s (%p)\n", wtitle, wtitle);
	g_print ("Label text: %s (%p)\n", ltext, ltext);
#endif

	gtk_window_set_title (window, ltext);
	gtk_label_set_text (label, wtitle);

	free (wtitle);
	free (ltext);
}

static
void activate(GtkApplication *app,
              gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *label;

	/* Create the window that can't be resized */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window title");
	gtk_widget_set_size_request (window, 300, 100);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

	/* Give some width to make the label visible */
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	/* Create the label */
	label = gtk_label_new ("Label text");
	gtk_label_set_selectable (GTK_LABEL (label), TRUE);

	/* Register the listener */
	g_signal_connect (window, "key-press-event", G_CALLBACK (swap_wtitle_and_ltext), (label));

	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (label));

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
