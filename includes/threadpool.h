#ifndef PROGETTO_THREADPOOL_H
#define PROGETTO_THREADPOOL_H

#include <pthread.h>

typedef struct threadpool {
    pthread_t *thread;  //array di thread
    int dim;    //dimensione del pool
} threadpool;

int init_threadpool(threadpool *pool, int nthread, void* (*task)(void *), void* arg);
int create_thread(threadpool *pool, void* (*task)(void *), void* arg);
int wait_threadpool(threadpool *pool);
int delete_threadpool(threadpool *pool);


#endif //PROGETTO_THREADPOOL_H
