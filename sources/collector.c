#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>

#include "results.h"
#include "config.h"

int create_socket();

volatile int stop_printing = 0;

void* print_results_task(void* arg) {
    resultarray* array = (resultarray*)arg;
    while (!stop_printing) {
        print_result(*array);
        sleep(1);
    }
    return NULL;
}

int start_collector(){
    printf("Collector in avvio...\n");

    //gestisco i segnali
    //TODO: gestire i segnali

    //creo il socket per la comunicazione con il MasterWorker
    int listenfd = create_socket();
    if(listenfd == -1){
        unlink(SOCKET_PATH);
        printf("Collector terminato\n");
        return -1;
    }

    //inizializzo la lista dei risultati da stampare
    resultarray array;
    if(init_resultarray(&array) != 0){
        printf("Errore nell'array 2\n");
        //TODO: gestire l'errore
        return -1;
    }

    // Avvia il thread di stampa
    pthread_t print_thread;
    if (pthread_create(&print_thread, NULL, print_results_task, &array) != 0) {
        perror("Errore nella creazione del thread di stampa");
        return -1;
    }

    int max_fd=0;
    if(listenfd > max_fd)
        max_fd = listenfd;

    //utilizzo la select per gestire le richieste di connessione
    fd_set set, rdset;
    FD_ZERO(&set);
    FD_SET(listenfd, &set);

    int stop = 0;

    while(!stop){
        rdset = set;
        int fd_ready;
        fd_ready = select(max_fd + 1, &rdset, NULL, NULL, NULL);
        if (fd_ready == -1){
            perror("Errore nella select");
            return -1;
        }

        int fd_curr = 0;
        int fds_found = 0;
        while(fds_found < fd_ready && fd_curr < max_fd + 1){
            if(!FD_ISSET(fd_curr, &rdset)){
                fd_curr++;
                continue;
            }
            else if(fd_curr == listenfd){
                int tmp_fd = accept(listenfd, NULL, 0);
                if(tmp_fd == -1){
                    perror("Errore nella accept");
                    return -1;
                }
                FD_SET(tmp_fd, &set);
                if(tmp_fd > max_fd)
                    max_fd = tmp_fd;
            }else {
                result msg;
                ssize_t nread = read(fd_curr, &msg, sizeof(result));
                if(nread == -1){
                    perror("Errore nella lettura del messaggio");
                    return -1;
                }
                if(nread == 0){
                    close(fd_curr);
                    FD_CLR(fd_curr, &set);
                    printf("Connessione chiusa\n");
                }else{
                    if(msg.value == -1 && msg.filepath[0] == '\0'){
                        // gestisci la terminazione
                        stop = 1;
                        break;

                    } else {
                        // gestisci il risultato come prima
                        add_result(array, msg.value, msg.filepath);
                        //print_result(array);
                    }
                }
            }
            fd_curr++;
            fds_found++;
        }

    }
    //fermo il thread di stampa facendolo stampare l'ultima volta
    stop_printing = 1;
    pthread_join(print_thread, NULL);
    print_result(array);R

    //chiudo tutti i socket
    for(int i = 0; i < max_fd + 1; i++){
        if(FD_ISSET(i, &set)){
            close(i);
            FD_CLR(i, &set);
        }
    }

    //chiudo il socket
    close(listenfd);

    //elimino il socket
    unlink(SOCKET_PATH);

    //elimino l'array dei risultati
    delete_resultarray(array);

    //printf("MESSAGGIO ARRIVATO STOP\n");
    //sleep(100);
    printf("Collector terminato\n");
    return 0;
}

int create_socket(){
    unlink(SOCKET_PATH);
    int sockfd;
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("Errore nella creazione del socket");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("Errore nel binding del socket");
        return -1;
    }

    if(listen(sockfd, SOMAXCONN) == -1){
        perror("Errore nella listen");
        return -1;
    }

    return sockfd;
}