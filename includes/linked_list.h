#ifndef PROGETTO_LINKED_LIST_H
#define PROGETTO_LINKED_LIST_H

#include <stdlib.h>

typedef struct linkedlist {
    void *string;
    size_t dim;
    size_t size;
    size_t elem_size;
} linkedlist;

int init_list(linkedlist *list, size_t string_size);

#endif //PROGETTO_LINKED_LIST_H
