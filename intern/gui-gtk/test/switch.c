/**
 * A simple test application that creates a window
 * with a switch that toggles the title.
 */

#include <gtk/gtk.h>

/**
 * Signal handler for the active state of switch
 */
static
void toggle_title(GObject *switcher,
                  GParamSpec *spec,
                  gpointer user_data)
{
	GtkWindow *window = user_data;

	if (gtk_switch_get_active (GTK_SWITCH (switcher)))
	{
		gtk_window_set_title(window, "Hello window");
	}
	else
	{
		gtk_window_set_title(window, "");
	}
}

static
void activate(GtkApplication *app)
{
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *label;
	GtkWidget *switcher;

	/* Create the window */
	window = gtk_application_window_new (app);
//	gtk_window_set_title (GTK_WINDOW (window), "Hello window");   // let the switch handler take care of this
	gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	label = gtk_label_new ("Title");

	switcher = gtk_switch_new ();

	/* Create the grid and attach the label and switch to the grid */
	grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID (grid), 10); // see if this works without 'GTK_GRID (grid)' - NO
	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), switcher, 1, 0, 1, 1);

	// see what happens if we remove GTK_SWITCH () - NOTHING
	g_signal_connect (switcher, "notify::active", G_CALLBACK (toggle_title), window);

	/* Set the active state of the switch to true. This would result in the callback being called. */
	gtk_switch_set_active (GTK_SWITCH (switcher), TRUE);

	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (grid));

	gtk_widget_show_all (window);
}

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("com.ks.example.switch", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
