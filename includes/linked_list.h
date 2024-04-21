#ifndef PROGETTO_LINKED_LIST_H
#define PROGETTO_LINKED_LIST_H

#include <stdlib.h>

typedef struct linkedlist{
    void *string;
    size_t dim;    //numero di elementi nella lista
    size_t size;    //massima grandezza della lista
    size_t elem_size;   //massima grandezza di un path
}linkedlist;

int init_list(linkedlist* list);
int delete_list(linkedlist *list);
int add_file(linkedlist *list, char* file);
int get_file(linkedlist *list, char* file);

#endif //PROGETTO_LINKED_LIST_H
