#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "my_signal.h"

//variabili usate dai segnali
volatile sig_atomic_t interrupted;

int main() {
    printf("MasterWorker in avvio...\n");

    //creo la signal mask
    set_signal_mask();
    //registro i gestori
    register_handlers();

    //sblocco i segnali
    unmask();
    while(!interrupted){
        fprintf(stdout, "in attesa\n");
        sleep(3);
    }
    printf("\nRicevuto segnale di terminazione\n");
    return 0;
}


