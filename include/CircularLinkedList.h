#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

struct node_t {
	void *data;                 // data is a generic pointer.
	struct node_t *next;
	struct node_t *prev;
};

/**
   circularlist_create -- Creates an initial node_t for the basis of a
                          circularlist.

   Returns -- A new node_t with an unintialized data pointer.
  */
struct node_t *circularlist_create();
// -----------------------------------------------------------------------------

/**
   circularlist_insert -- Insert data into an empty node_t. Create new node_t if
                          necessary.
   Parameters
       lnode -- A node_t apart of the list to be modified.
       data  -- A pointer to data of arbitrary type.
   Note
       Data must be malloc'd to heap prior to being passed as an argument.
 */
void
circularlist_insert(struct node_t **lnode, void *data);
// -----------------------------------------------------------------------------

/**
    circularlist_delete -- Deletes a node_t from the linked list. Data stored in
                           deleted node_t is freed first.
    Parameters
        remove -- The node_t to be deleted.
 */
void
circularlist_delete(struct node_t** remove);
// -----------------------------------------------------------------------------

/**
   circularlist_find -- Finds and returns a node_t using a function passed as an
                        argument (as the compare argument).
   Parameters
       lnode   -- A node_t apart of the list to be modified.
       query   -- data that is expected to be found in the linked list.
       compare -- function used to find query in linked list.
   Returns
       node_t containing query data.
   Note
       The node_t returned is determined when (*compare)(query, node_t->data)
       returns 0.

 */
struct node_t*
circularlist_find(struct node_t* lnode,
                  void* query,
                  int (*compare)(const void*, const void*));
// -----------------------------------------------------------------------------

/**
   circularlist_compare_strings -- A function to be used with circularlist_find
                                   (as the "compare" argument). Use when node_t
                                   data point to strings.
   Parameters
       s1, s2 -- strings
   Returns
       Follows same semantics as strcmp.
       < 0 when s1 appears before s2 in dictionary order.
       > 0 when s1 appears after s2 in dictionary order.
       = 0 when s1 and s2 are the same.
  */
int
circularlist_compare_strings(const void* s1, const void* s2);
// -----------------------------------------------------------------------------

/**
   circularlist_compare_integers -- A function to be used with circularlist_find
                                    (as the "compare" argument). Use when node_t
                                    data points to integers.
   Parameters
       p, q -- integers
   Returns
       0 when p == q
       1 when p != q
  */
int
circularlist_compare_integers(const void* p, const void* q);
// -----------------------------------------------------------------------------

/**
   circularlist_traverse -- A function that will apply the callback() function
                            on every element in the list.
   Parameters
       lnode    -- A node_t apart of the list to be traversed.
       callback -- function that is applied to each element of the list which
                   lnode is apart of.
  */
void
circularlist_traverse(struct node_t* lnode, void (*callback)(struct node_t*));
// -----------------------------------------------------------------------------

/**
   circularlist_print -- A function intended for use with circularlist_traverse,
                         which assumes each node_t's data points to a string.
   Parameters
       lnode    -- A node_t apart of a list.
  */
void
circularlist_print(struct node_t* lnode);
// -----------------------------------------------------------------------------

#endif // CIRCULAR_LINKED_LIST_H
