#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "master.h"
#include "my_signal.h"
#include "linked_list.h"
#include "util.h"
#include "config.h"
#include "concurrent_queue.h"


int get_options(int argc, char* argv[], int* nthread, int* qlen, int* delay, char *directory, linkedlist argfiles, bool *directory_set);
int create_connection();


int master_worker(int argc, char *argv[]){
    interrupted = 0;

    int nthread = NUM_THREADS, qlen = QUEUE_SIZE, delay = DELAY;
    char directory[MAX_PATH_LEN + 1];
    bool directory_set = false;

    linkedlist argfiles;
    concurrent_queue queue;

    init_list(&argfiles);

    //recupero gli argomenti passati al programma
    get_options(argc, argv, &nthread, &qlen, &delay, directory, argfiles, &directory_set);

    printf("Valori impostati:\t");
    if(directory_set){printf("nthread: %d, qlen: %d, delay: %d, directory: %s\n", nthread, qlen, delay, directory);}
    else {printf("nthread: %d, qlen: %d, delay: %d\n", nthread, qlen, delay);}

    //inizializzo la coda concorrente dei task
    if(init_queue(&queue, qlen) != 0){
        print_error("Errore nell'inizializzazione della coda concorrente\n");
        clear_all(argfiles, queue);
        return -1;
    }

    //creo il processo Collector
    int collector_pid;
    if((collector_pid = fork()) == 0){
        start_collector();
        exit(0);
    }

    //stabilisco la connessione con il processo Collector creando il socket
    int sock_collector;
    sock_collector = create_connection();
    if(sock_collector == -1){
        clear_all(argfiles, queue);
        return -1;
    }

    //Invio un messaggio Hello al Collector
    char hello_msg[] = "HELLO";
    if(writen(sock_collector, &hello_msg, strlen(hello_msg) + 1) != strlen(hello_msg) + 1){
        print_error("Errore nell'invio del messaggio HELLO al Collector\n");
        clear_all(argfiles, queue);
        return -1;
    }
    printf("Messaggio inviato\n");

    sleep(300);


    //sblocco i segnali
    //unmask();
    clear_all(argfiles, queue);
    return 0;
}

int create_connection(){
    int sockfd;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
    addr.sun_family = AF_UNIX;

    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        print_error("Errore nella creazione del socket\n");
        return -1;
    }

    int attempts = 0;
    while(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        if(attempts++ >= MAX_ATTEMPTS){
            print_error("Impossibile connettersi al socket dopo %d tentativi\n", MAX_ATTEMPTS);
            return -1;
        }
        printf("Sto tentando di connettermi al socket...\n");
        sleep(3);
        continue;
    }
    printf("Connessione al socket riuscita\n");
    return sockfd;
}

int get_options(int argc, char* argv[], int *nthread, int *qlen, int *delay, char *directory, linkedlist argfiles, bool *directory_set){
    int opt;
    while((opt = getopt(argc, argv, "n:q:t:d:")) != -1){
        switch(opt){
            case 'n':
                if(checkPosNum(optarg, nthread, "il valore di -n non e' un numero positivo\n") != 0){
                    return -1;
                }
                break;
            case 'q':
                if(checkPosNum(optarg, qlen, "il valore di -q non e' un numero positivo\n") != 0){
                    return -1;
                }
                break;
            case 't':
                if(checkPosNum(optarg, delay, "il valore di -t non e' un numero positivo\n") != 0){
                    return -1;
                }
                break;
            case 'd': {
                if (strlen(optarg) > MAX_PATH_LEN) {
                    print_error("il nome della directory supera la lunghezza massima consentita\n");
                    return -1;
                }
                strcpy(directory, optarg);
                printf("directory: <%s>\tProcedo ad esplorarla\n", directory);
                *directory_set = true;
                explore_dir(directory, argfiles);
                break;
            }
            default:
                print_error("Opzione non riconosciuta\n");
                return -1;

        }
    }

    // Processa gli argomenti rimanenti (i nomi dei file)
    struct stat file_stat;
    for(int i = optind; i < argc; i++){
        //controllo se il file è un file regolare
        if(stat(argv[i], &file_stat) != 0){
            print_error("Errore nella lettura delle informazioni del file %s, procedo a ignorarlo\n", argv[i]);
            //se non lo è, lo ignoro e passo al successivo
            continue;
        }
        if(!S_ISREG(file_stat.st_mode)){
            print_error("Il file %s non è un file regolare\n", argv[i]);
            return -1;
        }
        //aggiungo il file alla lista
        if(add_file(argfiles, argv[i]) != 0){
            print_error("Errore nell'aggiunta del file %s alla lista\n", argv[i]);
            return -1;
        }
    }

    return 0;
}

void explore_dir(char *directory, linkedlist argfiles){
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char path[MAX_PATH_LEN + 1];
    if((dir = opendir(directory)) == NULL){
        print_error("Errore nell'apertura della directory %s\n", directory);
        return;
    }
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        if(strlen(directory) + strlen(entry->d_name) + 2 > MAX_PATH_LEN){
            print_error("La lunghezza del percorso per il file %s nella directory %s supera la lunghezza massima consentita\n", entry->d_name, directory);
            continue;
        }
        if((snprintf(path, MAX_PATH_LEN, "%s/%s", directory, entry->d_name)) > MAX_PATH_LEN){
            print_error("Errore nella creazione del path per il file %s\n", entry->d_name);
            continue;
        }
        if(stat(path, &file_stat) != 0){
            print_error("Errore nella lettura delle informazioni del file %s\n", path);
            //ignoro il file e passo al successivo
            continue;
        }
        if(S_ISDIR(file_stat.st_mode)){
            explore_dir(path, argfiles);
        }
        //se è un file regolare aggiungo il path del file alla lista
        else if(S_ISREG(file_stat.st_mode)){
            if(add_file(argfiles, path) != 0){
                print_error("Errore nell'aggiunta del file %s alla lista\n", path);
            }
        }
    }
    closedir(dir);
}

void clear_all(linkedlist argfiles, concurrent_queue queue){
    delete_list(argfiles);
    delete_queue(queue);
}


