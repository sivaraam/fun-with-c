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
	static const char *label_css_path = "label-styles.css";
	GFile *label_css_file;
	GtkCssProvider *label_css;
	GError *label_style_load_error = NULL;

	/* Create the window that can't be resized */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window title");
	gtk_widget_set_size_request (window, 300, 100);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

	/* Give some width to make the label visible */
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	/* Create the label */
	label = gtk_label_new ("Label text");
//	gtk_label_set_selectable (GTK_LABEL (label), TRUE);

	/* Initialise the styles for the label */
	label_css_file = g_file_new_for_path (label_css_path);
	label_css = gtk_css_provider_new ();
	gtk_css_provider_load_from_file (label_css, label_css_file, &label_style_load_error);

	if (label_style_load_error == NULL)
	{
		gtk_style_context_add_provider (gtk_widget_get_style_context (label),
	                                        GTK_STYLE_PROVIDER (label_css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}
	else
	{
		g_print ("Loading styles failed with error: %s\n", label_style_load_error->message);
	}

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
