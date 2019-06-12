#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct List {
    int len;
    struct Node *head;
};
struct ScaleInfo {
    char *name;
    char *intervals;
};
struct Node {
    struct ScaleInfo scale;
    struct Node *next;
};

struct List *createList ();
void addToList (struct List **list, char *name, char *intervals);
void printList (struct List **list);

struct ScaleInfo *getInfo (struct List **list);

#endif // LINKED_LIST_H
