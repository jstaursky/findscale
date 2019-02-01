#include <cairo.h>
#include <gtk/gtk.h>
#include <string.h>

struct imagelayers {
    cairo_surface_t *instrument;
    cairo_surface_t *notes[12];
};

enum {Ab, A, Bb, B, C, Db, D, Eb, E, F, Gb, G};

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
    struct imagelayers *temp = user_data;
    cairo_set_source_surface(cr, temp->instrument, 0, 0);

    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_mask_surface(cr, temp->notes[A], 0, 0);
    cairo_fill(cr);

    gtk_widget_set_size_request(widget,
                                cairo_image_surface_get_width(temp->instrument),
                                cairo_image_surface_get_height(temp->instrument));

    return FALSE;
}


struct imagelayers*
getinstrumentlayers(char *instru_choice)
{
    char* chromaticscale[] = {"Ab","A","Bb","B","C","Db","D","Eb","E","F","Gb","G"};

    struct imagelayers* images =
            (struct imagelayers*)malloc(sizeof(struct imagelayers));

    char *instru_loc = (char*)malloc(
            sizeof(char) *
            (strlen("imgs/") + strlen(instru_choice) + strlen(".png")));
    sprintf(instru_loc, "imgs/%s.png", instru_choice);
    images->instrument = cairo_image_surface_create_from_png(instru_loc);

    for (int note = 0; note < 12; ++note) {
        char *note_loc = (char*)malloc(
            sizeof(char) *
            (strlen("imgs/") + strlen(instru_choice) + strlen("-")
             + strlen(chromaticscale[note]) + strlen("-notes.png")) + 1);

        sprintf(note_loc, "imgs/%s-%s-notes.png", instru_choice, chromaticscale[note]);
        images->notes[note] =
            cairo_image_surface_create_from_png(note_loc);
    }

    return images;

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

    /* Must provide instrument of choice as cmdline argument. */
    struct imagelayers* images = getinstrumentlayers(argv[1]);

    /* image.instrument = */
    /*     cairo_image_surface_create_from_png(instru_loc); */

    /* image.notes = cairo_image_surface_create_from_png("imgs/piano-A-notes.png"); */

    g_signal_connect(G_OBJECT(draw_area), "draw",
                     G_CALLBACK(on_draw_event), (gpointer)images);


    /* --------------------------------------------------------------------- */
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
