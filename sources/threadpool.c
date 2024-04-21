#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "threadpool.h"


//work -> i thread prendono i file dalla coda e calcolano il risultato
//arg -> la coda da cui prendono i file
int init_threadpool(threadpool *pool, int nthread, void* (*task)(void *), void* arg){

    pool->dim = 0;
    pool->desired_threads = nthread;

    if((pool->thread = malloc(sizeof(pthread_t) * nthread)) == NULL){
        return -1;
    }

    pool->args = malloc(sizeof(thread_args));
    pool->args->pool = pool;
    pool->args->queue = (concurrent_queue *)arg;

    for(int i = 0; i < nthread; i++){
        if(pthread_create(&(pool)->thread[i], NULL, task, pool->args) != 0){
            return -1;
        }
        ++pool->dim;
    }

    pthread_mutex_init(&pool->mutex, NULL);
    return 0;
}

int create_thread(threadpool *pool, void* (*task)(void *), void* arg){
    if(pool == NULL){
        return -1;
    }

    //creo un thread nel pool
    //incremento la dimensione del pool

    pool->args = malloc(sizeof(thread_args));
    pool->args->pool = pool;
    pool->args->queue = (concurrent_queue *)arg;

    if(pthread_create(&(pool)->thread[pool->dim], NULL, task, pool->args) != 0){
        return -1;
    }

    ++pool->dim;
    ++pool->desired_threads;
    printf("dim: %d\n", pool->dim);

    return 0;
}

int delete_thread(threadpool *pool, concurrent_queue *queue){
    if(pool == NULL){
        return -1;
    }

    //se il pool è vuoto non posso eliminare thread
    if(pool->dim == 1 || pool == NULL){
        return -1;
    }

    //elimino un thread dal pool
    //decremento la dimensione del pool

    --pool->desired_threads;

    printf("dim: %d\n", pool->dim);

    return 0;
}



int wait_threadpool(threadpool *pool){
    for(int i = 0; i < pool->dim; i++){
        if(pthread_join(pool->thread[i], NULL) != 0){
            return -1;
        }
    }
    return 0;
}

int delete_threadpool(threadpool *pool){
    if(pool == NULL){
        return -1;
    }
    free(pool->args);
    free(pool->thread);
    return 0;
}

int get_thread_index(threadpool *pool){
    if(pool == NULL){
        return -1;
    }
    pthread_t self = pthread_self();
    for (int i = 0; i < pool->dim; i++) {
        if (pthread_equal(pool->thread[i], self)) {
            return i;
        }
    }
    return -1;  // Return -1 if the current thread is not found in the pool
}

