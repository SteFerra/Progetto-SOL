
#ifndef PROGETTO_MASTER_H
#define PROGETTO_MASTER_H


#include "linked_list.h"

int master_worker(int argc, char *argv[]);

void clear_all(linkedlist argfiles);

void explore_dir(char *directory, linkedlist argfiles);

#endif //PROGETTO_MASTER_H
