#ifndef PROGETTO_LINKED_LIST_H
#define PROGETTO_LINKED_LIST_H

#include <stdlib.h>

typedef struct linkedlist *linkedlist;

int init_list(linkedlist* list);
int delete_list(linkedlist list);
int add_file(linkedlist list, char* file);
int get_file(linkedlist list, char* file);
int is_empty_list(linkedlist list);
int get_list_size(linkedlist list);
int print_list(linkedlist list);

#endif //PROGETTO_LINKED_LIST_H
