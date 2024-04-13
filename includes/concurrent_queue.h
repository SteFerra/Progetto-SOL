#ifndef PROGETTO_CONCURRENT_QUEUE_H
#define PROGETTO_CONCURRENT_QUEUE_H

typedef struct concurrent_queue *concurrent_queue;

int init_queue(concurrent_queue *queue, size_t size);

void delete_queue(concurrent_queue queue);

#endif //PROGETTO_CONCURRENT_QUEUE_H
