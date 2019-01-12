#include <cairo.h>
#include <gtk/gtk.h>

struct data {
    cairo_surface_t *piano;
    cairo_surface_t *notes;     /* TODO: Turn this into linked list of
                                 * cairo_surface_t* elements */
};

/*
 * Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
on_draw_event(GtkWidget *widget,
              cairo_t *cr,
              gpointer user_data)
{
    struct data *temp = user_data;
    cairo_set_source_surface(cr, temp->piano, 0, 0);

    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_mask_surface(cr, temp->notes, 0, 0);
    cairo_fill(cr);

    gtk_widget_set_size_request(widget,
                                cairo_image_surface_get_width(temp->piano),
                                cairo_image_surface_get_height(temp->piano));

    return FALSE;
}

int
main(int   argc,
     char *argv[])
{
    /* Create window, set position, title, etc. */
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position     (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW(window), 300, 220);
    gtk_window_set_title        (GTK_WINDOW(window), "Title");
    /* --------------------------------------------------------------------- */
    GtkWidget *draw_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw_area);

    struct data image;

    image.piano = cairo_image_surface_create_from_png("imgs/piano.png");
    image.notes = cairo_image_surface_create_from_png("imgs/piano-A-notes.png");

    g_signal_connect(G_OBJECT(draw_area), "draw",
                     G_CALLBACK(on_draw_event), (gpointer)&image);


    /* --------------------------------------------------------------------- */
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
