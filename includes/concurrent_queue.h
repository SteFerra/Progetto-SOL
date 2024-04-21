#ifndef PROGETTO_CONCURRENT_QUEUE_H
#define PROGETTO_CONCURRENT_QUEUE_H

#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

typedef struct {
    char **buffer;  // Array di stringhe per memorizzare i nomi dei file
    size_t capacity;   // Capacità massima della coda
    size_t size;       // Numero corrente di elementi nella coda
    size_t front;      // Indice del primo elemento
    size_t rear;       // Indice dell'ultimo elemento
    pthread_mutex_t mutex;      // Mutex per la sincronizzazione
    pthread_cond_t not_empty;  // Condizione per coda vuota
    pthread_cond_t not_full;   // Condizione per coda piena
    pthread_cond_t empty;      // Condizione per coda vuota
    bool terminate;             // Flag per terminare il thread
} concurrent_queue;

int init_queue(concurrent_queue *queue, size_t size);
int delete_queue(concurrent_queue *queue);
int add_file_queue(concurrent_queue *queue, char *file);
int get_file_queue(concurrent_queue *queue, char *file);
int wait_for_empty_queue(concurrent_queue *queue);
int set_terminate(concurrent_queue *queue);

void print_queue(concurrent_queue queue);

#endif //PROGETTO_CONCURRENT_QUEUE_H