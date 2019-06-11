#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

struct List *createList ()
{
    struct List *list = malloc (sizeof (struct List));
    list->len = 0;
    list->head = NULL;
    return list;
}

static struct Node *createNode (char *name, char *intervals)
{
    struct Node *node = malloc (sizeof (struct Node));
    char *storeName = malloc (sizeof (char) * (strlen (name) + 1));
    char *storeIntervals = malloc (sizeof (char) * (strlen (intervals) + 1));

    strcpy (storeName, name);
    strcpy (storeIntervals, intervals);

    node->scale.name = storeName;
    node->scale.intervals = storeIntervals;
    node->next = NULL;
    return node;
}

void addToList (struct List **list, char *name, char *intervals)
{
    struct Node *node;
    node = createNode (name, intervals);

    if (!(*list)) {
        puts ("Fail: list is NULL");
        return;
    }
    if (!(*list)->head) {
        (*list)->head = node;
    } else {
        node->next = (*list)->head;
        (*list)->head = node;
    }

    (*list)->len += 1;
}

static void traverseList (struct List **list, void (*callback) (struct Node *))
{
    struct Node *current = (*list)->head;
    do {
        (*callback) (current);
    } while (current = current->next, current != NULL);

    return;
}

static void printNode (struct Node *node)
{
    puts (node->scale.name);
    puts (node->scale.intervals);
}

void printList (struct List **list)
{
    traverseList (list, printNode);
}

struct ScaleInfo *getInfo (struct List **list)
{
    struct ScaleInfo *info = malloc (sizeof(struct ScaleInfo) * (*list)->len);
    struct Node *current = (*list)->head;
    for (int i = 0; i < (*list)->len; current = current->next, ++i) {
        info[i].name = current->scale.name;
        info[i].intervals = current->scale.intervals;
    }

    return info;

}
