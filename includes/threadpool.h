#ifndef PROGETTO_THREADPOOL_H
#define PROGETTO_THREADPOOL_H

#include <pthread.h>
#include <signal.h>
#include "concurrent_queue.h"

struct threadpool;

typedef struct thread_args {
    struct threadpool *pool;
    concurrent_queue *queue;
} thread_args;

typedef struct threadpool {
    pthread_t *thread;  //array di thread
    int dim;    //dimensione del pool
    int desired_threads;    //numero di thread desiderati
    thread_args *args;
    pthread_mutex_t mutex;
} threadpool;

int init_threadpool(threadpool *pool, int nthread, void* (*task)(void *), void* arg);
int create_thread(threadpool *pool, void* (*task)(void *), void* arg);
int delete_thread(threadpool *pool, concurrent_queue *queue);
int wait_threadpool(threadpool *pool);
int delete_threadpool(threadpool *pool);


#endif //PROGETTO_THREADPOOL_H
