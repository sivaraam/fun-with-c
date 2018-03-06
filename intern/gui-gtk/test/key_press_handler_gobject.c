/**
 * A simple test application that swaps the window title
 * with the label text when any key is pressed.
 *
 * Functions related GObject are used.
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

#ifdef DEBUG
	g_print ("sizeof (\"Window title\"): %zu\n", sizeof ("Window title"));
#endif

	static const size = 15;
	char *wtitle = malloc (size);
	char *ltext  = malloc (size);

	g_object_get (G_OBJECT (window), "title", &wtitle, NULL);
	g_object_get (G_OBJECT (label), "label", &ltext, NULL);

#ifdef DEBUG
	g_print ("Window title: %s (%p)\n", wtitle, wtitle);
	g_print ("Label text: %s (%p)\n", ltext, ltext);
#endif

	g_object_set (G_OBJECT (window), "title", ltext, NULL);
	g_object_set (G_OBJECT (label), "label", wtitle, NULL);

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
	g_object_set (G_OBJECT (window), "title", "Window title", NULL);
	g_object_set (G_OBJECT (window), "default-width", 300, NULL);
	g_object_set (G_OBJECT (window), "default-height", 100, NULL);
	g_object_set (G_OBJECT (window), "resizable", FALSE, NULL);

	/* Give some width to make the label visible */
	g_object_set (G_OBJECT (window), "border-width", 10, NULL);

	/* Create the label */
	label = gtk_label_new ("Label text");
	g_object_set (G_OBJECT (label), "selectable", TRUE, NULL);

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
