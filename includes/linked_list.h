#ifndef PROGETTO_LINKED_LIST_H
#define PROGETTO_LINKED_LIST_H

#include <stdlib.h>

typedef struct linkedlist *linkedlist;

int init_list(linkedlist* list);
int delete_list(linkedlist list);
int add_file(linkedlist list, char* file);
int get_file(linkedlist list, char* file);

void print_list(linkedlist list);

#endif //PROGETTO_LINKED_LIST_H
