#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/* Local Library(s) */
#include "list.h"
/* ***TODO***
 *
 * - Need to add a "destructor" to free memory on heap utilized by
 *   list_createnode and list_prepend.
 */

//  --------------------------------------------------------------------------
//  Create very first node of an eventual linked list.

node_t*
list_createnode(void *Data)
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


//  --------------------------------------------------------------------------
//  Inserts a node before "next" node and stores "data" in it.

status_t
list_prepend(node_t *next,
             void   *data)
{
    if (!next)
        return ERROR;

    node_t *node_new = list_createnode(data);
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

//  --------------------------------------------------------------------------
//  Append a node after the "back" node, initialize with "data".

status_t
list_append(node_t *back,
            void   *data)
{
    if (!back)
        return ERROR;

    node_t *node_new = list_createnode(data);
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
        /* Handle non-singleton lists. */
        NEXT(node_new) = NEXT(back);
        BACK(NEXT(back)) = node_new;
    }
    NEXT(back) = node_new;
    return OK;
}

status_t
list_traverse(node_t *node,
              void  (*list_map)(node_t *))
{
    node_t *cur = node;
    do {
        (*list_map)(cur);
    } while (cur = NEXT(cur), cur != node);

    return OK;
}
