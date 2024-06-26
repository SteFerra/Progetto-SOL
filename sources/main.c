#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "util.h"
#include "master.h"
#include "my_signal.h"

int main(int argc, char *argv[]) {
    //maschero i segnali
    sigset_t set;
    set_signal_mask(&set);

    if(argc < 2){
        PRINT_HELP
        return -1;
    }

    //processo MasterWorker
    pid_t MasterWorker_pid = fork();
    if(MasterWorker_pid == 0){
        if(master_worker(argc, argv) != 0){
            return -1;
        }
    }

    //attendo la terminazione del processo MasterWorker
    int status;
    waitpid(MasterWorker_pid, &status, 0);

    return 0;
}


