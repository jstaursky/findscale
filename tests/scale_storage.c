#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

char*
fgetLine(FILE *stream);

struct scale_t {
    char *name;
    char *intervals;
};

int
main(int argc,
     char *argv[])
{
    FILE *fp = fopen(argv[1], "r");

    node_t *head = List_createnode(NULL);

    for (char *line; (line = fgetLine(fp));) {
        struct scale_t *scale = malloc(sizeof(struct scale_t));

        scale->name = strtok(line, ",\t");
        scale->intervals = strtok(NULL, ",\040\t");

        List_prepend(head, scale);
    }
    node_t *cur = NEXT(head);
    while (DATA(cur)) {
        puts((*((struct scale_t *)DATA(cur))).name);
        cur = NEXT(cur);
    }
}

char*
fgetLine(FILE *stream)
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
    int c;                      /* fgetc returns int. Comparing EOF w/ char may
                                 * cause issues. */
    while ( (c = fgetc(stream)) != EOF &&
            (*ptr = c) != '\n')
    {
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
