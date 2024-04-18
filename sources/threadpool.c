#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "threadpool.h"


//work -> i thread prendono i file dalla coda e calcolano il risultato
//arg -> la coda da cui prendono i file
int init_threadpool(threadpool *pool, int nthread, void* (*task)(void *), void* arg){

    pool->dim = 0;

    if((pool->thread = malloc(sizeof(pthread_t) * nthread)) == NULL){
        return -1;
    }

    for(int i = 0; i < nthread; i++){
        if(pthread_create(&(pool)->thread[i], NULL, task, arg) != 0){
            return -1;
        }
        ++pool->dim;
    }
    return 0;
}

int create_thread(threadpool *pool, void* (*task)(void *), void* arg){
    if(pool == NULL){
        return -1;
    }
    if(pthread_create(&pool->thread[pool->dim], NULL, task, arg) != 0){
        return -1;
    }
    ++pool->dim;
    printf("Thread creato\n");
    printf("Numero di thread: %d\n", pool->dim);
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

