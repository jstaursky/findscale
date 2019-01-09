#include <stdio.h>
#include <string.h>
#include "nodemodule.h"

int
main(int   argc,
     char *argv[])
{

    node_t *head = List_createnode(NULL);

    if (argc == 1) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < strlen(argv[1]); ++i) {
        List_append(head, (void *)argv[1][i]);
    }


    node_t *cur = head;

    do {
        putchar((char)DATA(cur));
       cur = cur->back;
    } while (cur != head);

    do {
        putchar((char)DATA(cur));
        cur = cur->next;
    } while (cur != head);
}
