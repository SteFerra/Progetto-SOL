#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


#include "my_signal.h"
#include "util.h"
#include "master.h"

void* sig_handler(void* arg){
    sigset_t set;

    set_signal_mask();
    register_handlers();
    //creo la signal mask e registro i segnali

    int s, sig;
    for(;;){
        s = sigwait(&set, &sig);
        if(s!=0){
            print_error("Errore nella sigwait\n");
            exit(-1);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("MasterWorker in avvio...\n");

    //controllo che il numero di argomenti sia almeno 2 (nome del programma e lista di file)
    if(argc < 2){
        PRINT_HELP;
        return -1;
    }

    //processo MasterWorker
    pid_t MasterWorker_pid = fork();
    if(MasterWorker_pid == 0){

        pthread_t sig_thread;
        int ret = pthread_create(&sig_thread, NULL, &sig_handler, NULL );
        if(ret != 0){
            print_error("Errore nella creazione del thread per la gestione dei segnali\n");
            return -1;
        }

        return master_worker(argc, argv);
    }

    //attendo la terminazione del processo MasterWorker
    int status;
    waitpid(MasterWorker_pid, &status, 0);

    printf("MasterWorker terminato\n");
    return 0;
}


