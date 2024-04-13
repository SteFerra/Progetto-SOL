#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include "my_signal.h"

volatile sig_atomic_t interrupted;

void set_signal_mask(){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGHUP);    //codice 1
    sigaddset(&set, SIGINT);    //codice 2
    sigaddset(&set, SIGQUIT);   //codice 3
    sigaddset(&set, SIGTERM);   //codice 15
    sigaddset(&set, SIGUSR1);   //codice 10
    sigaddset(&set, SIGUSR2);   //codice 12
    pthread_sigmask(SIG_SETMASK, &set, NULL);
}

void register_handlers(){
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = termination_handler;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    //TO-DO aggiungere SIGUSR1 e SIGUSR2

}

void unmask(){
    interrupted = 0;
    sigset_t set;
    sigemptyset(&set);
    pthread_sigmask(SIG_SETMASK, &set, NULL);
}

void termination_handler(int signum){
    interrupted = 1;
    printf("Ricevuto segnale di terminazione\n");
}

