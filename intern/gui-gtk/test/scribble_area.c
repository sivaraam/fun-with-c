/**
 * A simple test application that creates a scribble area.
 *
 * Ref: Custom Drawing <https://developer.gnome.org/gtk3/stable/ch01s05.html>
 */

#include <gtk/gtk.h>

#define DEBUG
/**
 * Surface to store current scribbles.
 */
static cairo_surface_t *surface = NULL;

/**
 * Initialize the surface to white.
 */
static
void clear_surface (void)
{
	cairo_t *cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);

	cairo_destroy (cr);
}

/**
 * Create a new surface of the appropriate size to store our scribbles.
 */
static
gboolean configure_event_cb (GtkWidget *widget,
                             GdkEvent *event,
                             gpointer data)
{
	if (surface)
	{
		cairo_surface_destroy (surface);
	}

	surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
	                                             CAIRO_CONTENT_COLOR,
	                                             gtk_widget_get_allocated_width (widget),
	                                             gtk_widget_get_allocated_height (widget));

	clear_surface();

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/**
 * Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static
gboolean draw_cb (GtkWidget *widget,
                  cairo_t *cr,
                  gpointer data)
{
	cairo_set_source_surface (cr, surface, 0, 0);
	cairo_paint (cr);

	return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static
void draw_brush (GtkWidget *widget,
                 gdouble x,
                 gdouble y)
{
	cairo_t *cr = cairo_create (surface);

	cairo_rectangle (cr, x-3, y-3, 6, 6);
	cairo_fill (cr);

	cairo_destroy (cr);

	/* Invalidate the area currently drawn */
	gtk_widget_queue_draw_area (widget, x-3, y-3, 6, 6);

}

static
gboolean button_press_event_cb (GtkWidget *widget,
                                GdkEventButton *event,
                                gpointer data)
{
	/* paranoia check. Just in case, we haven't got the surface to draw */
	if (surface == NULL)
	{
		return FALSE;
	}

#ifdef DEBUG
	g_print ("About to handle 'button click' event ..\n");
#endif

	if (event->button == GDK_BUTTON_PRIMARY)
	{
		draw_brush (widget, event->x, event->y);

#ifdef DEBUG
		g_print ("Got x: %lf, y: %lf\n", event->x, event->y);
#endif

	}
	else if (event->button == GDK_BUTTON_SECONDARY)
	{
		clear_surface ();
		gtk_widget_queue_draw (widget);
	}

	/* We've handled the event, stop processing */
	return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static
gboolean motion_notify_event_cb (GtkWidget *widget,
                                 GdkEventMotion *event,
                                 gpointer data)
{
	/* paranoia check. Just in case, we haven't got the surface to draw */
	if (surface == NULL)
	{
		return FALSE;
	}

#ifdef DEBUG
	g_print ("About to handle 'motion notify' event ..\n");
#endif

	if (event->state & GDK_BUTTON1_MASK)
	{
		draw_brush (widget, event->x, event->y);

#ifdef DEBUG
		g_print ("Got x: %lf, y: %lf\n", event->x, event->y);
#endif

	}

	/* We've handled it, stop processing */
	return TRUE;
}

static
void destroy_surface(void)
{
	if (surface)
	{
		cairo_surface_destroy (surface);
	}
}

static
void activate(GtkApplication *app,
              gpointer user_data)
{
	GtkWidget *window, *frame, *drawing_area;

	/* Create the window */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Drawing area");

	gtk_container_set_border_width (GTK_CONTAINER (window), 8);

	g_signal_connect (window, "destroy", G_CALLBACK (destroy_surface), NULL);

	/* Create a frame to hold the scribble surface and add it to the window */
	frame = gtk_frame_new (NULL);
	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
	gtk_container_add (GTK_CONTAINER (window), frame);

	/* Create a drawing area with a minimum size of 100*100
	 * and add it to the frame
	 */
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_area, 100, 100);
	gtk_container_add (GTK_CONTAINER (frame), drawing_area);

	/* Signals used to handle the backing surface */
	/* Handle the window drawing and size change events. */
	g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
	g_signal_connect (drawing_area, "configure-event", G_CALLBACK (configure_event_cb), NULL);

	/* Event signals */
	g_signal_connect (drawing_area, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), NULL);
	g_signal_connect (drawing_area, "button-press-event", G_CALLBACK (button_press_event_cb), NULL);

	gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
	                                     | GDK_POINTER_MOTION_MASK
                                             | GDK_BUTTON_PRESS_MASK);

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
