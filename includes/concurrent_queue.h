#ifndef PROGETTO_CONCURRENT_QUEUE_H
#define PROGETTO_CONCURRENT_QUEUE_H

typedef struct concurrent_queue *concurrent_queue;

int init_queue(concurrent_queue *queue, size_t size);
void delete_queue(concurrent_queue queue);
int add_file_queue(concurrent_queue queue, char *file);
int get_file_queue(concurrent_queue *queue, char *file);

void print_queue(concurrent_queue queue);

#endif //PROGETTO_CONCURRENT_QUEUE_H
