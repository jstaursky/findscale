#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "CircularLinkedList.h"
#include "gtk/gtkwidget.h"

#define DEBUG_CHECK

struct scale_t {
    char *name;
    struct node_t *scale;
    int num_notes;
};
struct database_t {
    struct scale_t **entry;
    int size;
};
struct imagelayers_t {
    cairo_surface_t *instrument;
    cairo_surface_t *notes[12]; // chromaticscale is largest scale.
    int note_count;
};


struct database_t *build_database(FILE *);
struct imagelayers_t *getimages(struct scale_t *, char *, char *);
static gboolean on_draw_event(GtkWidget *, cairo_t *, gpointer fn_parameter);
char *fgetline(FILE *stream);


int main(int argc, char *argv[])
{
    // Create window, set position, title, etc. reference
    // https://developer.gnome.org/gtk-tutorial/stable/c450.html for more info
    // on creating widgets.
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 220);
    gtk_window_set_title(GTK_WINDOW(window), "Findscale");
    // Handle exit signal.
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    // Create container for packing widgets in a single row.
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create draw area.
    // reference https://developer.gnome.org/gtk3/stable/GtkDrawingArea.html for
    // more info on gtk_drawing_area_new()
    GtkWidget *draw_area = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(box), draw_area, TRUE, TRUE, 10);

    // Import list of scales.
    FILE *configfp = fopen("conf/scale.list", "r");
    struct database_t *scale_database = build_database(configfp);

#ifdef DEBUG_CHECK
    // Print results of import.
    for (int i = 0; i < scale_database->size; ++i) {
        circularlist_traverse(scale_database->entry[i]->scale,
                              circularlist_print);
    }
#endif
    // Get the images associated with database entry 0.
    struct imagelayers_t *displaylayers = getimages(scale_database->entry[0],
                                                    argv[1], argv[2]);

    // Draw the image layers just received which correspond to database entry 0.
    // reference https://developer.gnome.org/gtk-tutorial/stable/x159.html for
    // more info on 'g_signal_connect'.
    g_signal_connect(G_OBJECT(draw_area), "draw",
                     *G_CALLBACK(on_draw_event), (gpointer)displaylayers);


    // Create widget to choose scale from database.
    GtkWidget *list_box = gtk_combo_box_text_new();
    gtk_box_pack_start(GTK_BOX(box), list_box, FALSE, TRUE, 0);
    for (int i = 0; i < scale_database->size; ++i) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(list_box),
                                       scale_database->entry[i]->name);
    }


    /* Begin running of GUI. ----------------------------------------- */
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}



struct database_t *build_database(FILE *fp)
{
    struct database_t *db = malloc(sizeof(struct database_t));
    db->entry = NULL;

    int db_idx = 0;
    for (char *line; (line = fgetline(fp)); ++db_idx) {
        db->entry = realloc(db->entry, sizeof(struct scale_t *) * (db_idx + 1));
        db->entry[db_idx] = malloc(sizeof(struct scale_t));
        db->entry[db_idx]->scale = circularlist_create();

        char *rest = line;
        db->entry[db_idx]->name = strtok_r(line, ",", &rest);
        while (isspace(*rest))
            ++rest;

        char *interval;
        int note_count = 0;
        while ((interval = strtok_r(NULL, "-", &rest))) {
            circularlist_insert(&db->entry[db_idx]->scale, interval);
            ++note_count;
        }
        db->entry[db_idx]->num_notes = note_count;
    }
    db->size = db_idx;

    return db;
}

// getimages returns an array holding the images relating to contents found in
// scale.
struct imagelayers_t *getimages(struct scale_t *scale_p, char *instrument_name,
                                char *keysig)
{
    static char *chromaticscale[] = { "Ab", "A",  "Bb", "B", "C",  "Db",
                                      "D",  "Eb", "E",  "F", "Gb", "G" };
    // TODO need to make keys selectable from menu, or something more elegant
    // than this.
    int key = 0;
    while (strcmp(keysig, chromaticscale[key]) != 0) {
        ++key;
    }

    struct imagelayers_t *images = malloc(sizeof(struct imagelayers_t));

    char *instru_loc =
        malloc(sizeof(char) *
               (strlen("imgs/") + strlen(instrument_name) + strlen(".png")));
    sprintf(instru_loc, "imgs/%s.png", instrument_name);

    images->instrument = cairo_image_surface_create_from_png(instru_loc);
    cairo_surface_reference(images->instrument);

    int idx = 0;
    struct node_t *scaleproxy = scale_p->scale;

    do {
        int note = key + atoi(scaleproxy->data);
        note %= 12;
        char *note_loc =
            malloc(sizeof(char) * (strlen("imgs/") + strlen(instrument_name) +
                                   strlen("-") + strlen(chromaticscale[note]) +
                                   strlen("-notes.png")) +
                   1);

        sprintf(note_loc, "imgs/%s-%s-notes.png", instrument_name,
                chromaticscale[note]);

        images->notes[idx] = cairo_image_surface_create_from_png(note_loc);
        cairo_surface_reference(images->notes[idx]);

        ++idx;
    } while ((scaleproxy = scaleproxy->next) != scale_p->scale);

    images->note_count = idx;

    return images;
}

/*
 * Callback function used with g_signal_connect().
 * Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean on_draw_event(GtkWidget *widget, // Widget emitting a signal.
                              cairo_t *cr,       // Signal being emitted.
                              gpointer fn_parameter)
{
    struct imagelayers_t *layers = fn_parameter;

    cairo_set_source_surface(cr, layers->instrument, 0, 0);
    cairo_paint(cr);

    for (int i = 0; i < layers->note_count; ++i) {
        cairo_set_source_surface(cr, layers->notes[i], 0, 0);
        cairo_paint(cr);
    }

    gtk_widget_set_size_request(
        widget, cairo_image_surface_get_width(layers->instrument),
        cairo_image_surface_get_height(layers->instrument));

    return FALSE;
}

char *fgetline(FILE *stream)
{
    const size_t chunk = 128;
    size_t max = chunk;

    /* Preliminary check */
    if (!stream || feof(stream))
        return NULL;

    char *buffer = (char *)malloc(chunk * sizeof(char));
    if (!buffer) {
        perror("Unable to allocate space");
        return NULL;
    }
    char *ptr = buffer;
    int c; /* fgetc returns int. Comparing EOF w/ char may
            * cause issues. */
    while ((c = fgetc(stream)) != EOF && (*ptr = c) != '\n') {
        ++ptr;
        size_t offset = ptr - buffer;
        if (offset >= max) {
            max += chunk;

            char *tmp = realloc(buffer, max);
            if (!tmp) {
                free(buffer);
                return NULL;
            }
            buffer = tmp;
            ptr = tmp + offset;
        }
    }
    *ptr = '\0';
    return buffer;
}
