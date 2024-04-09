#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


#include "my_signal.h"
#include "util.h"
#include "master.h"

//variabili usate dai segnali
volatile sig_atomic_t interrupted;

int main(int argc, char *argv[]) {
    printf("MasterWorker in avvio...\n");

    //controllo che il numero di argomenti sia almeno 2 (nome del programma e lista di file)
    if(argc < 2){
        PRINT_HELP;
        return -1;
    }

    //creo la signal mask e registro i segnali
    set_signal_mask();
    register_handlers();

    //processo MasterWorker
    pid_t MasterWorker_pid = fork();
    if(MasterWorker_pid == 0){
        return master_worker(argc, argv);
    }

    //attendo la terminazione del processo MasterWorker
    int status;
    waitpid(MasterWorker_pid, &status, 0);

    printf("MasterWorker terminato\n");
    return 0;
}


