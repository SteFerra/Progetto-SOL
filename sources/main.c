#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


#include "my_signal.h"
#include "util.h"
#include "master.h"

int main(int argc, char *argv[]) {
    printf("MasterWorker in avvio...\n");

    if(argc < 2){
        PRINT_HELP;
        return -1;
    }

    //processo MasterWorker
    pid_t MasterWorker_pid = fork();
    if(MasterWorker_pid == 0){
        printf("PID MasterWorker: %d\n", getpid());
        return master_worker(argc, argv);
    }

    //attendo la terminazione del processo MasterWorker
    int status;
    waitpid(MasterWorker_pid, &status, 0);

    printf("MasterWorker terminato\n");
    return 0;
}


