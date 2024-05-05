#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "threadpool.h"

// inizializza il threadpool con il numero di thread dato da nthread e la funzione task
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

// funzione per creare un thread nel pool usata all'arrivo del segnale SIGUSR1
int create_thread(threadpool *pool, void* (*task)(void *), void* arg){
    if(pool == NULL){
        return -1;
    }

    pool->args = malloc(sizeof(thread_args));
    pool->args->pool = pool;
    pool->args->queue = (concurrent_queue *)arg;

    if(pthread_create(&(pool)->thread[pool->dim], NULL, task, pool->args) != 0){
        return -1;
    }

    ++pool->dim;
    ++pool->desired_threads;

    return 0;
}

// funzione per eliminare un thread dal pool usata all'arrivo del segnale SIGUSR2
int delete_thread(threadpool *pool, concurrent_queue *queue){
    if(pool == NULL){
        return -1;
    }

    //se il pool contiene un solo thread non posso eliminarlo
    if(pool->dim == 1){
        return -1;
    }

    --pool->desired_threads;

    return 0;
}


// funzione per attendere la terminazione di tutti i thread del pool
int wait_threadpool(threadpool *pool){
    for(int i = 0; i < pool->dim; i++){
        if(pthread_join(pool->thread[i], NULL) != 0){
            return -1;
        }
    }
    return 0;
}

// funzione per eliminare il pool e liberare la memoria
int delete_threadpool(threadpool *pool){
    if(pool == NULL){
        return -1;
    }
    free(pool->args);
    free(pool->thread);
    return 0;
}