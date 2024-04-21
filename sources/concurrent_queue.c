#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"
#include "concurrent_queue.h"


int init_queue(concurrent_queue *queue, size_t size) {
    if (queue == NULL) {
        return -1;
    }

    queue->capacity = size;
    queue->size = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->terminate = false;

    if(((queue->buffer) = malloc(sizeof(char *) * (queue->capacity))) == NULL){
        for(int i = 0; i < queue->capacity; i++){
            free(queue->buffer[i]);
        }
        free(queue->buffer);
        return -1;
    }

    for(int i = 0; i < queue->capacity; i++){
        queue->buffer[i] = malloc(sizeof(char) * (MAX_PATH_LEN + 1));
        if(queue->buffer[i] == NULL){
            for(int j = 0; j < i; j++){
                free(queue->buffer[j]);
            }
            free(queue->buffer);
            return -1;
        }
    }

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    pthread_cond_init(&queue->empty, NULL);

    return 0;
}

int add_file_queue(concurrent_queue *queue, char *item) {

    pthread_mutex_lock(&queue->mutex);

    while (queue->size == queue->capacity) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    if(queue->terminate == true){
        pthread_mutex_unlock(&queue->mutex);
        pthread_cond_signal(&queue->not_empty);
        return -1;
    }

    strncpy(queue->buffer[queue->rear], item, (MAX_PATH_LEN + 1));
    queue->rear++;
    queue->rear %= queue->capacity;
    queue->size++;

    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->not_empty);
    return 0;
}

// Estrazione di un elemento dalla coda
int get_file_queue(concurrent_queue *queue, char *file) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->size == 0 && queue->terminate == false) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }

    if(queue->terminate == true){
        pthread_mutex_unlock(&queue->mutex);
        if(queue->size == 0){
            pthread_cond_signal(&queue->empty);
        }
        pthread_cond_signal(&queue->not_full);
        return -1;
    }

    strncpy(file, queue->buffer[queue->front], (MAX_PATH_LEN +1));
    queue->front++;
    queue->front %= queue->capacity;
    queue->size--;

    pthread_mutex_unlock(&queue->mutex);

    if(queue->size == 0){
        pthread_cond_signal(&queue->empty);
    }

    //pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->not_full);

    return 0;
}

// Deallocazione della coda
int delete_queue(concurrent_queue *queue) {
    if(queue == NULL){
        return -1;
    }
    for (size_t i = 0; i < queue->capacity; i++) {
        if(queue->buffer[i] != NULL) {
            free(queue->buffer[i]);
            queue->buffer[i] = NULL;  // Set the pointer to NULL after freeing
        }
    }
    free(queue->buffer);
    queue->buffer = NULL;  // Set the pointer to NULL after freeing
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
    pthread_cond_destroy(&queue->empty);

    return 0;

}

// aspetta che la coda sia vuota
int wait_for_empty_queue(concurrent_queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->size > 0) {
        pthread_cond_wait(&queue->empty, &queue->mutex);
    }
    pthread_mutex_unlock(&queue->mutex);
    return 0;
}

int set_terminate(concurrent_queue *queue) {
    if(queue == NULL){
        return -1;
    }
    pthread_mutex_lock(&queue->mutex);
    queue->terminate = true;
    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_broadcast(&queue->not_empty);
    pthread_cond_broadcast(&queue->not_full);
    return 0;
}

void print_queue(concurrent_queue queue) {

    for (size_t i = 0; i < queue.capacity; i++) {
        printf("%s\n", queue.buffer[i]);
    }


}





