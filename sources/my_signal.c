#include <pthread.h>
#include <signal.h>
#include <string.h>
#include "my_signal.h"

volatile sig_atomic_t siginterruption_received;
volatile sig_atomic_t sigusr1_received;
volatile sig_atomic_t sigusr2_received;

// imposto la maschera dei segnali
void set_signal_mask(sigset_t *set){
    sigemptyset(set);
    sigaddset(set, SIGHUP);    //codice 1
    sigaddset(set, SIGINT);    //codice 2
    sigaddset(set, SIGQUIT);   //codice 3
    sigaddset(set, SIGTERM);   //codice 15
    sigaddset(set, SIGUSR1);   //codice 10
    sigaddset(set, SIGUSR2);   //codice 12
    pthread_sigmask(SIG_SETMASK, set, NULL);
}

void ignore_signals(){
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGPIPE, &sa, NULL);
}


