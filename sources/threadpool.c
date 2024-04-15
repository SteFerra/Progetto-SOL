#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

typedef struct threadpool {
    pthread_t *thread;  //array di thread
    int dim;    //dimensione del pool
} threadpool;


//work -> i thread prendono i file dalla coda e calcolano il risultato
//arg -> la coda da cui prendono i file
int init_threadpool(threadpool **pool, int nthread, void* (*task)(void *), void* arg){
    if((*pool = malloc(sizeof(threadpool))) == NULL){
        return -1;
    }
    (*pool)->dim = 0;

    if(((*pool)->thread = malloc(sizeof(pthread_t) * nthread)) == NULL){
        free(*pool);
        return -1;
    }

    for(int i = 0; i < nthread; i++){
        if(pthread_create(&(*pool)->thread[i], NULL, task, arg) != 0){
            free(*pool);
            return -1;
        }
    }
    (*pool)->dim = nthread;
    return 0;
}

int create_thread(threadpool **pool, void* (*task)(void *), void* arg){
    if(pool == NULL){
        return -1;
    }
    if(pthread_create(&(*pool)->thread[(*pool)->dim], NULL, task, arg) != 0){
        return -1;
    }
    (*pool)->dim++;
    printf("Thread creato\n");
    printf("Numero di thread: %d\n", (*pool)->dim);
    return 0;
}
