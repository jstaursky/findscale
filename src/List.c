#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/* Local Library(s) */
#include "List.h"
/* ***TODO***
 *
 * - Need to add a "destructor" to free memory on heap utilized by
 *   List_createnode and List_prepend.
 */


/**
 * Create a node.
 *
 * NOTE: The data structure built from the nodes is a Doubly-Linked circular
 * linked list.
 */
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

/**
 *
 * status_t List_prepend(node_t *next, void *data)
 *
 * List_prepend:
 *
 *    List_prepend will insert a new node before another node listed as an
 *    argument as well as store data in the new node.
 *
 * Parameters   : next -- A pointer to the node being prepended to.
 *                data -- data to be attached to inserted node.
 *
 * Return Value : An integer indicating whether operation was a success or not.
 *                NOTE status_t -- created via enum typedef.
 * Description:
 *
 *    This function inserts a new node infront the node listed in the argument.
 *    NOTE: list is a doubly-Linked circular linked list.
 *
 */
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

/**
 *
 * status_t List_append(node_t *back, void *data)
 *
 * List_append:
 *
 *    List_append will insert a new node after another node listed as an argument
 *    as well as store data in the new node.
 *
 * Parameters   : back -- A pointer to the node being appended to.
 *                data -- data to be attached to inserted node.
 *
 * Return Value : An integer indicating whether operation was a success or not.
 *                NOTE status_t -- created via enum typedef.
 *
 * Description:
 *
 *    This function inserts a new node behind the node listed in the argument.
 *    NOTE: list is a doubly-Linked circular linked list.
 *
 */
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
        /* Handle non-singleton lists. */
        NEXT(node_new) = NEXT(back);
        BACK(NEXT(back)) = node_new;
    }
    NEXT(back) = node_new;
    return OK;
}

/**
 *
 * status_t List_traverse(node_t *node, void (*List_map)(node_t *)
 *
 * List_traverse:
 *
 *    List_traverse applies the map provided by the function pointer List_map.
 *
 * Parameters   : node -- Arbitrary node in list used as a starting and end
 *                        point.
 *                List_map -- a function pointer which accepts functions with
 *                            node_t* arguments and return void. List_map is
 *                            applied to each node in the list.
 *
 * Return Value : An integer indicating whether operation was a success or not.
 *                NOTE status_t -- created via enum typedef.
 *
 * Description:
 *
 *    This function applies a map specified by the function pointer List_map to
 *    each node in list.
 *    NOTE: list is a doubly-Linked circular linked list.
 *
 */
status_t
List_traverse(node_t *node,
              void  (*List_map)(node_t *))
{
    node_t *cur = node;
    do {
        (*List_map)(cur);
    } while (cur = NEXT(cur), cur != node);

    return OK;
}
