#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

typedef struct concurrent_queue {
    char **tasks; //array di task
    size_t max_size; //dimensione massima della coda data da MAX_SIZE
    size_t size; //dimensione della coda data da qlen
    size_t start;
    size_t end;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty; //condizione che tiene traccia se la coda non è vuota
    pthread_cond_t not_full; //condizione che tiene traccia se la coda non è piena
} concurrent_queue;

int init_queue(concurrent_queue **queue, size_t size) {
    int i = 0;

    if((*queue = malloc(sizeof(concurrent_queue))) == NULL){
        return -1;
    }
    (*queue)->tasks = NULL;
    (*queue)->max_size = MAX_SIZE + 1;
    (*queue)->size = size;
    (*queue)->start = 0;
    (*queue)->end = 0;

    if(((*queue)->tasks = malloc(sizeof(char*) * (*queue)->size)) == NULL){
        free(*queue);
        return -1;
    }

    for(i = 0; i < (*queue)->size; i++){
        if(((*queue)->tasks[i] = malloc((*queue)->max_size)) == NULL){
            free(*queue);
            return -1;
        }
    }

    if(pthread_mutex_init(&(*queue)->mutex, NULL) != 0){
        free(*queue);
        return -1;
    }

    if(pthread_cond_init(&(*queue)->not_empty, NULL) != 0){
        pthread_mutex_destroy(&(*queue)->mutex);
        free(*queue);
        return -1;
    }

    printf("Coda concorrente inizializzata\n");
    return 0;
}

void delete_queue(concurrent_queue *queue) {
    if(queue == NULL){
        return;
    }
    for(int i = 0; i < queue->size; i++){
        free(queue->tasks[i]);
    }
    free(queue->tasks);

    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
    free(queue);
}

void add_file_queue(concurrent_queue *queue, char *file){
    if(queue == NULL || file == NULL){
        return;
    }
    pthread_mutex_lock(&queue->mutex);
    while(queue->end == queue->size){
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    strncpy(queue->tasks[queue->end], file, queue->max_size);
    queue->end = (queue->end + 1) % queue->size;


    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);


}

void print_queue(concurrent_queue *queue){
    if(queue == NULL){
        return;
    }
    for(int i = 0; i < queue->size; i++){
        printf("File <%s> in posizione %d\n", queue->tasks[i], i);
    }

}

