#include <getopt.h>
#include "master.h"
#include "my_signal.h"
#include "linked_list.h"
#include "util.h"


int get_options(int argc, char* argv[], int* nthread, int* qlen, int* delay, char *directory);

int master_worker(int argc, char *argv[]){
    interrupted = 0;
    int ret; //valore di ritorno

    int nthread, qlen, delay;
    char directory[MAX_PATH_LEN + 1];
    linkedlist argfiles;



    //recupero gli argomenti passati al programma
    if (ret = get_options(argc, argv, &nthread, &qlen, &delay, directory) != 0){
        //TODO: pulire la memoria;
        print_error("Errore nella lettura degli argomenti\n");
    }

    //sblocco i segnali
    unmask();
    return 0;
}

int get_options(int argc, char* argv[], int *nthread, int *qlen, int *delay, char *directory){
    int opt;
    while((opt = getopt(argc, argv, "n:q:t:d:")) != -1){
        switch(opt){
            case 'n':
                if(checkPosNum(optarg, nthread, "il valore di -n non e' un numero positivo\n") != 0){
                    return -1;
                }
                printf("nthread: %d\n", *nthread);
                break;
            case 'q':
                if(checkPosNum(optarg, qlen, "il valore di -q non e' un numero positivo\n") != 0){
                    return -1;
                }
                printf("qlen: %d\n", *qlen);
                break;
            case 't':
                if(checkPosNum(optarg, delay, "il valore di -t non e' un numero positivo\n") != 0){
                    return -1;
                }
                printf("delay: %d\n", *delay);
                break;
            case 'd': {
                if (strlen(optarg) > MAX_PATH_LEN) {
                    print_error("il nome della directory supera la lunghezza massima consentita\n");
                    return -1;
                }
                strcpy(directory, optarg);
                printf("directory: %s\n", directory);
                break;
            }
            default:
                print_error("Opzione non riconosciuta\n");
                return -1;

        }
    }

    // Processa gli argomenti rimanenti (i nomi dei file)
    for(int i = optind; i < argc; i++){
        //TODO: aggiungere i file alla lista
    }

    return 0;
}


