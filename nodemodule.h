#ifndef NODEMODULE_H
#define NODEMODULE_H

#define DATA(L) ((L)->data)
#define NEXT(L) ((L)->next)
#define BACK(L) ((L)->back)

#include <stdlib.h>
#include <errno.h>

typedef enum { OK,    ERROR } status_t;
typedef enum { FALSE, TRUE  } bool;

typedef struct node_t {
	void          *data;
	struct node_t *back;
	struct node_t *next;
} node_t;                       /* Create node_t type. */

/* Create a node_t. */
node_t*
List_createnode(void *Data)
{
	node_t *node_new = (node_t *)malloc(sizeof(node_t));
	if (!node_new) {
        perror("Unable to allocate node_t.");
		return NULL;
    }
	DATA(node_new) = Data;

    /* Leave this assignment to other functions. */
    NEXT(node_new) = NULL;
    BACK(node_new) = NULL;

    return node_new;
}

status_t
List_prepend(node_t *next,
             void   *data)
{
    if (!next)
        return ERROR;

    node_t *node_new = List_createnode(data);
    if (!node_new) {
        perror("Unable to allocate node_t.");
        return ERROR;
    }
    DATA(node_new) = data;
    NEXT(node_new) = next;

    /* If BACK(next) is NULL then 'next' node_t is the only node in the list. */
    if (!BACK(next)) {
        BACK(node_new) = next;
        NEXT(next) = node_new;
    } else {
        /* When BACK(next) is not NULL store this into BACK(node_new).. */
        BACK(node_new) = BACK(next);

        /* Prepending to 'next' node is same as appending to the node originally
         * pointed to by BACK(next). */
        NEXT(BACK(next)) = node_new;
    }
    /* Now update BACK(next) to point to the new prepended node. */
    BACK(next) = node_new;
    return OK;
}

status_t
List_append(node_t *back,
            void   *data)
{
    if (!back)
        return ERROR;

    node_t *node_new = List_createnode(data);
    if (!node_new) {
        perror("Unable to allocate node_t.");
        return ERROR;
    }
    DATA(node_new) = data;
    BACK(node_new) = back;

    /* Handle the case of a single node in list. */
    if (!BACK(back)) {
        NEXT(node_new) = back;
        BACK(back) = node_new;
    } else {
        NEXT(node_new) = NEXT(back);
        BACK(NEXT(back)) = node_new;
    }
    NEXT(back) = node_new;
    return OK;
}

#endif /* NODEMODULE_H */
