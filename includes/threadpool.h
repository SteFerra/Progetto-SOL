#ifndef PROGETTO_THREADPOOL_H
#define PROGETTO_THREADPOOL_H

typedef struct threadpool *threadpool;

int init_threadpool(threadpool *pool, int dim, void* (*task)(void *), void* arg);
int create_thread(threadpool *pool, void* (*task)(void *), void* arg);


#endif //PROGETTO_THREADPOOL_H
