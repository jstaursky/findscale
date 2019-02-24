#include <cairo.h>
#include <gtk/gtk.h>
#include <string.h>
#include "list.h"

struct imagelayers {
	cairo_surface_t *instrument;
	cairo_surface_t *notes[12];
        unsigned int count;
};

typedef struct scale_t {
	char *name;
	node_t *intervals;
} scale_t;

#define SCALE_NAME(node) (*((scale_t *)DATA(node))).name
#define SCALE(node) (*((scale_t *)DATA(node))).intervals

enum { Ab, A, Bb, B, C, Db, D, Eb, E, F, Gb, G };

char*
fgetline(FILE *stream);

struct imagelayers*
getinstrumentlayers(char   *instru_choice,
                    int     key,
                    node_t *intervals);

static gboolean
on_draw_event(GtkWidget *widget,
              cairo_t   *cr,
              gpointer   user_data);


int
main(int argc, char *argv[])
{
	// Create window, set position, title, etc.
	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 220);
	gtk_window_set_title(GTK_WINDOW(window), "Title");
	/* ------------------------------------------------------------------ */
	GtkWidget *draw_area = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), draw_area);

	// Import list of scales.
	FILE *configfp = fopen("conf/scale.list", "r");
	// Create initial node of scale listing.
	node_t *scalelist_h = list_createnode(NULL);

	for (char *line; line = fgetline(configfp);) {
		scale_t *scale = malloc(sizeof(scale_t));
		scale->intervals = list_createnode(NULL);
		scale->name = strtok(line, ",\t");

		char *noteinterval = strtok(strtok(NULL, ",\040\t"), "-");
		while (noteinterval != NULL) {
			// Two digits + null is maximum string size
			char *note = malloc(sizeof(char) * 3);
			strcpy(note, noteinterval);
			if (!DATA(scale->intervals)) {
				DATA(scale->intervals) = note;
			} else {
				list_prepend(scale->intervals, note);
			}
			noteinterval = strtok(NULL, "-");
		}

		if (!DATA(scalelist_h)) {
			DATA(scalelist_h) = scale;
		} else {
			list_prepend(scalelist_h, scale);
		}
	}
	// Import complete, access scales through scalelist_h;

	node_t *tmp = SCALE(scalelist_h);
	int count = 1;          // count the number of notes in scale.
	while ((tmp = NEXT(tmp)) != SCALE(scalelist_h)) {
		count++;
                puts(DATA(tmp));
	}

	// Must provide instrument of choice as cmdline argument.
	struct imagelayers *images =
		getinstrumentlayers(argv[1], D, SCALE(scalelist_h));

	g_signal_connect(G_OBJECT(draw_area), "draw", G_CALLBACK(on_draw_event),
			 (gpointer)images);

	/* ------------------------------------------------------------------ */
	gtk_widget_show_all(window);
	gtk_main();
	return 0;

} /* main */

char*
fgetline(FILE *stream)
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

struct imagelayers*
getinstrumentlayers(char   *instru_choice,
                    int     key,
                    node_t *intervals)
{
	char *chromaticscale[] = {"Ab", "A",  "Bb", "B", "C",  "Db",
				  "D",  "Eb", "E",  "F", "Gb", "G"};

	struct imagelayers *images = malloc(sizeof(struct imagelayers));

	// instrument image layer located in imgs/<instrument>.png
	char *instru_loc = malloc(
		sizeof(char)
		* (strlen("imgs/") + strlen(instru_choice) + strlen(".png")));

	sprintf(instru_loc, "imgs/%s.png", instru_choice);
	images->instrument = cairo_image_surface_create_from_png(instru_loc);

        unsigned int cnt = 0;
	node_t *tmp = intervals;
	while ((tmp = NEXT(tmp)) != intervals) {
		int note = key + atoi(DATA(tmp));
		if (note > 12) {
			note -= 12;
		}
		char *note_loc = malloc(
			sizeof(char)
				* (strlen("imgs/") + strlen(instru_choice)
				   + strlen("-") + strlen(chromaticscale[note])
				   + strlen("-notes.png"))
			+ 1);

		sprintf(note_loc, "imgs/%s-%s-notes.png", instru_choice,
			chromaticscale[note]);

		images->notes[cnt] =
			cairo_image_surface_create_from_png(note_loc);
                ++cnt;
	}
        images->count = cnt + 1; // +1 to account for starting at 0.
        printf("%d\n", cnt);
	return images;
}

/*
 * Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean on_draw_event(GtkWidget *widget,
                              cairo_t   *cr,
                              gpointer   user_data)
{
	struct imagelayers *temp = user_data;

	cairo_set_source_surface(cr, temp->instrument, 0, 0);
	cairo_paint(cr);

	for (int i = 0; i < temp->count; ++i) {
		cairo_set_source_surface(cr, temp->notes[i], 0, 0);
		cairo_paint(cr);
	}

	gtk_widget_set_size_request(
		widget, cairo_image_surface_get_width(temp->instrument),
		cairo_image_surface_get_height(temp->instrument));

	return FALSE;
}
