
#ifndef PROGETTO_MASTER_H
#define PROGETTO_MASTER_H


#include "linked_list.h"
#include "concurrent_queue.h"

int master_worker(int argc, char *argv[]);

void clear_all(linkedlist argfiles, concurrent_queue queue);

void explore_dir(char *directory, linkedlist argfiles);

int start_collector();

#endif //PROGETTO_MASTER_H
