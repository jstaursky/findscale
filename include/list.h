/* List interface */
#ifndef LIST_H
#define LIST_H

#define DATA(L) ((L)->data)
#define NEXT(L) ((L)->next)
#define BACK(L) ((L)->back)

typedef enum { OK,    ERROR } status_t;
typedef enum { FALSE, TRUE  } bool;

typedef struct node_t {
    void          *data;
    struct node_t *back;
    struct node_t *next;
} node_t;

/**
 * Create a node.
 *
 * NOTE: The data structure built from the nodes is a Doubly-Linked circular
 * linked list.
 */
node_t*
list_createnode(void *Data);

/**
 *
 * status_t list_prepend(node_t *next, void *data)
 *
 * list_prepend:
 *
 *    list_prepend will insert a new node before another node listed as an
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
list_prepend(node_t *next, void *data);

/**
 *
 * status_t list_append(node_t *back, void *data)
 *
 * list_append:
 *
 *    list_append will insert a new node after another node listed as an
 *    argument as well as store data in the new node.
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
list_append(node_t *back, void *data);

/**
 *
 * status_t list_traverse(node_t *node, void (*list_map)(node_t *)
 *
 * list_traverse:
 *
 *    list_traverse applies the map provided by the function pointer list_map.
 *
 * Parameters   : node     -- Arbitrary node in list used as a starting and end
 *                            point.
 *                list_map -- a function pointer which accepts functions with
 *                            node_t* arguments and return void. list_map is
 *                            applied to each node in the list.
 *
 * Return Value : An integer indicating whether operation was a success or not.
 *                NOTE status_t -- created via enum typedef.
 *
 * Description:
 *
 *    This function applies a map specified by the function pointer list_map to
 *    each node in list.
 *    NOTE: list is a doubly-Linked circular linked list.
 *
 */
status_t
list_traverse(node_t *headnode, void (*list_map)(node_t *));

#endif /* LIST_H */
