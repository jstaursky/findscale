#include <gtk/gtk.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>
#include <glib/gprintf.h>

static gboolean drawArea_draw (GtkWidget *widget, cairo_t *cr,
                                gpointer *usr_arg)
{
    GdkPixbuf *pix_scaled, *pixbuf = *usr_arg;
    GdkWindow *window;
    gint width, height;

    window = gtk_widget_get_window (widget);
    height = gdk_window_get_height (window);
    // Calculate width such that aspect ratio is retained during rescale.
    width = (gdk_pixbuf_get_width (pixbuf) * height) /
            gdk_pixbuf_get_height (pixbuf);

    pix_scaled =
        gdk_pixbuf_scale_simple (pixbuf, width, height, GDK_INTERP_BILINEAR);

    gdk_cairo_set_source_pixbuf (cr, pix_scaled, 0, 0);
    cairo_paint (cr);

    g_object_unref (pix_scaled);

    return FALSE;
}

gchar *getResourceFileName(gchar *keysig, gchar *interval)
{
    if (keysig == NULL || interval == NULL)
        return NULL;
    gchar *fname;

    //   g_snprinf (fname, 80, "%s")

    return g_strdup(fname);
}

void
configurePixbufRender(GdkPixbuf** pixbuf, gchar *keysig, gchar *interval)
{
    GdkPixbuf* buf;
    cairo_surface_t* paint_dest;
    cairo_t* cr;
    cairo_surface_t* paint_src;

    buf = gdk_pixbuf_new_from_file("imgs/piano.png", NULL);

    paint_dest = cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, gdk_pixbuf_get_width(buf), gdk_pixbuf_get_height(buf));
    cr = cairo_create(paint_dest);
    cairo_surface_destroy(paint_dest);

    gdk_cairo_set_source_pixbuf(cr, buf, 0, 0);
    cairo_paint(cr);

    gchar *fname = getResourceFileName(keysig, interval);

    paint_src = cairo_image_surface_create_from_png(fname); // e.g. "piano-A-notes.png"
    if (cairo_surface_status (paint_src) == CAIRO_STATUS_SUCCESS) {
        cairo_set_source_surface(cr, paint_src, 0, 0);
        cairo_paint(cr); // write to paint_dest.
    } else {
        puts ("paint_src error");
    }

    *pixbuf =
        gdk_pixbuf_get_from_surface(paint_dest, 0, 0,
                                    cairo_image_surface_get_width(paint_dest),
                                    cairo_image_surface_get_height(paint_dest));
    // Clean up.
    cairo_surface_destroy(paint_src);

    cairo_destroy(cr);
}

int main (int argc, char **argv)
{
    GtkWidget *window, *container, *draw_area;
    GdkPixbuf *pixbuf;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    container = gtk_layout_new (NULL, NULL);
    draw_area = gtk_drawing_area_new ();

    configurePixbufRender (&pixbuf, NULL, NULL);

    gtk_window_set_default_size (GTK_WINDOW (window), 975, 260);

    // Signals
    g_signal_connect (G_OBJECT (draw_area), "draw", G_CALLBACK (drawArea_draw),
                      (gpointer)&pixbuf);

    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit),
                      NULL);

    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start (GTK_BOX (box), draw_area, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (window), box);

    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}
