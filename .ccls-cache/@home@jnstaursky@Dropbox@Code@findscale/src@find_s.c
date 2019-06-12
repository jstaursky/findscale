#include <gtk/gtk.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>

#include <string.h>
#include <ctype.h>

// gcc -o find_s find_s.c `pkg-config --cflags --libs gtk+-3.0`
static char *chromaticscale[] = { "Ab", "A",  "Bb", "B", "C",  "Db",
                                  "D",  "Eb", "E",  "F", "Gb", "G" };

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

int getNoteValue (char *note);

int getNoteCount (gchar *intervals)
{
    int count = 0;
    while (intervals = strstr (intervals, "-")) {
        ++count;
        intervals += strlen ("-");
    }
    return count;
}

gchar **getResourceFileNames (gchar *keysig, gchar *intervals)
{
    if (keysig == NULL || intervals == NULL)
        return NULL;

    int count = getNoteCount (intervals);

    gchar **intervalsSplit = g_strsplit (intervals, "-", count);

    int numValues[count];
    for (int i = 0; i < count; ++i) {
        numValues[i] = (getNoteValue (keysig) + atoi (intervalsSplit[i])) % 12;
        printf ("%d numValue\n", numValues[i]);
    }

    gchar **results = malloc (sizeof (gchar *) * count);

    for (int i = 0; i < count; ++i) {
        results[i] = malloc (sizeof (gchar) * 80);
        g_snprintf (results[i], 80, "imgs/piano-%s-notes.png",
                    chromaticscale[numValues[i]]);
        puts (results[i]);
    }

    return results;
}

void configurePixbufRender (GdkPixbuf **pixbuf, gchar *keysig, gchar *intervals)
{
    GdkPixbuf *pianoPixbuf;
    cairo_surface_t *paint_dest;
    cairo_t *cr;
    cairo_surface_t *paint_src;

    pianoPixbuf = gdk_pixbuf_new_from_file ("imgs/piano.png", NULL);

    paint_dest =
        cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                    gdk_pixbuf_get_width (pianoPixbuf),
                                    gdk_pixbuf_get_height (pianoPixbuf));
    cr = cairo_create (paint_dest);
    cairo_surface_destroy (paint_dest);

    gdk_cairo_set_source_pixbuf (cr, pianoPixbuf, 0, 0);
    cairo_paint (cr);

    // will need to loop this

    gchar **layers = getResourceFileNames (keysig, intervals);
    int layercount = getNoteCount (intervals);

    for (int i = 0; i < layercount; ++i) {
        printf ("INSIDE configurePixbufRender, layers[%d] is %s\n", i,
                layers[i]);
        paint_src = cairo_image_surface_create_from_png (
            layers[i]); // e.g. "piano-A-notes.png"
        free (layers[i]);
        if (cairo_surface_status (paint_src) == CAIRO_STATUS_SUCCESS) {
            cairo_set_source_surface (cr, paint_src, 0, 0);
            cairo_paint (cr); // write to paint_dest.
        } else {
            puts ("paint_src error");
        }
    }
    free (layers);

    *pixbuf = gdk_pixbuf_get_from_surface (
        paint_dest, 0, 0, cairo_image_surface_get_width (paint_dest),
        cairo_image_surface_get_height (paint_dest));
    // Clean up.
    cairo_surface_destroy (paint_src);

    cairo_destroy (cr);
}

static void listBoxKey_change (GtkWidget *keyWidget, gpointer usr_arg[])
{
    GHashTable *table = usr_arg[0];
    GtkWidget *listBoxScaleNames = usr_arg[1];
    GtkWidget *draw_area = usr_arg[2];
    GdkPixbuf **pixbuf = usr_arg[3];

    gchar *scaleName = gtk_combo_box_text_get_active_text (
        GTK_COMBO_BOX_TEXT (listBoxScaleNames));
    gchar *key =
        gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (keyWidget));

    puts (key);
    int keyval = getNoteValue (key);
    printf ("%d\n", keyval);

    // If there is nothing selected inside Scale name selectbox, do nothing.
    if (!scaleName) {
        return;
    }
    gchar *intervals = g_hash_table_lookup (table, scaleName);
    puts (intervals);

    configurePixbufRender (pixbuf, key, intervals);

    gtk_widget_queue_draw (draw_area);
}

static void listBoxScale_change (GtkWidget *scaleWidget, gpointer usr_arg[])
{
    GHashTable *table = usr_arg[0];
    GtkWidget *listBoxKeySignatures = usr_arg[1];
    GtkWidget *draw_area = usr_arg[2];
    GdkPixbuf **pixbuf = usr_arg[3];

    gchar *scaleName =
        gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (scaleWidget));
    gchar *key = gtk_combo_box_text_get_active_text (
        GTK_COMBO_BOX_TEXT (listBoxKeySignatures));

    gchar *intervals = g_hash_table_lookup (table, scaleName);
    puts (intervals);

    configurePixbufRender (pixbuf, key, intervals);

    gtk_widget_queue_draw (draw_area);
}

char *fgetline (FILE *stream);

void populateKeysigWidget (GtkWidget **widget)
{
    for (int i = 0; i < 12; ++i) { // A Ab ...
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(*widget),
                                       chromaticscale[i]);
    }
}

int main (int argc, char **argv)
{
    GtkWidget *window, *draw_area;
    GdkPixbuf *pixbuf;
    GtkWidget *box;
    GtkWidget *keysigBox;
    GtkWidget *scaleNameBox;
    GHashTable *scaleDefTable;

    gtk_init(&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    draw_area = gtk_drawing_area_new ();

    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start (GTK_BOX (box), draw_area, TRUE, TRUE, 0);

    keysigBox = gtk_combo_box_text_new ();
    gtk_box_pack_start (GTK_BOX (box), keysigBox, FALSE, TRUE, 0);

    populateKeysigWidget(&keysigBox);


    scaleNameBox = gtk_combo_box_text_new ();
    gtk_box_pack_start (GTK_BOX (box), scaleNameBox, FALSE, TRUE, 0);

    scaleDefTable = g_hash_table_new (g_str_hash, g_str_equal);

    FILE *configfp = fopen ("conf/scale.list", "r");

    for (char *line; (line = fgetline (configfp));) {
        char *name, *intervals;
        char *rest = line;
        name = strtok_r (line, ",", &rest);
        while (isspace (*rest))
            ++rest;
        intervals = rest;

        g_hash_table_insert (scaleDefTable, g_strdup (name), g_strdup (intervals));
        gchar *result = g_hash_table_lookup (scaleDefTable, name);
        puts (result);
    }

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init (&iter, scaleDefTable);
    while (g_hash_table_iter_next (&iter, &key, &value)) {
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (scaleNameBox), key);
    }

    gtk_combo_box_set_active (GTK_COMBO_BOX (keysigBox), 0);
    gtk_combo_box_set_active (GTK_COMBO_BOX (scaleNameBox), 0);

    gchar *initialKey =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(keysigBox));
    gchar *initialScale =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(scaleNameBox));

    configurePixbufRender(&pixbuf, initialKey,
                          g_hash_table_lookup(scaleDefTable, initialScale));

    gtk_window_set_default_size(GTK_WINDOW(window), 975 + 200, 260);


    g_signal_connect (GTK_COMBO_BOX_TEXT (keysigBox), "changed",
                      G_CALLBACK (listBoxKey_change),
                      ((gpointer[]){ scaleDefTable,
                                     scaleNameBox,
                                     draw_area,
                                     &pixbuf }));

    g_signal_connect (GTK_COMBO_BOX_TEXT (scaleNameBox), "changed",
                      G_CALLBACK (listBoxScale_change),
                      ((gpointer[]){ scaleDefTable,
                                     keysigBox,
                                     draw_area,
                                     &pixbuf }));

    g_signal_connect (G_OBJECT (draw_area), "draw",
                      G_CALLBACK (drawArea_draw),
                      (gpointer)&pixbuf);

    g_signal_connect (G_OBJECT (window), "destroy",
                      G_CALLBACK (gtk_main_quit),
                      NULL);

    gtk_container_add (GTK_CONTAINER (window), box);

    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}

int getNoteValue (char *note)
{
    int key = 0;
    while (strcmp (note, chromaticscale[key]) != 0) {
        ++key;
    }
    return key;
}

char *fgetline (FILE *stream)
{
    const size_t chunk = 128;
    size_t max = chunk;

    /* Preliminary check */
    if (!stream || feof (stream))
        return NULL;

    char *buffer = (char *)malloc (chunk * sizeof (char));
    if (!buffer) {
        perror ("Unable to allocate space");
        return NULL;
    }
    char *ptr = buffer;
    int c; /* fgetc returns int. Comparing EOF w/ char may
          * cause issues. */
    while ((c = fgetc (stream)) != EOF && (*ptr = c) != '\n') {
        ++ptr;
        size_t offset = ptr - buffer;
        if (offset >= max) {
            max += chunk;

            char *tmp = realloc (buffer, max);
            if (!tmp) {
                free (buffer);
                return NULL;
            }
            buffer = tmp;
            ptr = tmp + offset;
        }
    }
    *ptr = '\0';
    return buffer;
}
