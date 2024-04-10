#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include "master.h"
#include "my_signal.h"
#include "linked_list.h"
#include "util.h"


int get_options(int argc, char* argv[], int* nthread, int* qlen, int* delay, char *directory, linkedlist argfiles);

int master_worker(int argc, char *argv[]){
    interrupted = 0;
    int ret; //valore di ritorno

    int nthread, qlen, delay;
    char directory[MAX_PATH_LEN + 1];
    linkedlist argfiles;
    init_list(&argfiles);

    //recupero gli argomenti passati al programma
    if (ret = get_options(argc, argv, &nthread, &qlen, &delay, directory, argfiles) != 0){
        clear_all(argfiles);
        print_error("Errore nella lettura degli argomenti\n");
    }


    //sblocco i segnali
    unmask();
    return 0;
}

int get_options(int argc, char* argv[], int *nthread, int *qlen, int *delay, char *directory, linkedlist argfiles){
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
                printf("directory: %s\nProcedo ad esplorarla\n", directory);
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
            print_error("Errore nella lettura delle informazioni del file %s\n", argv[i]);
            return -1;
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
            print_error("Path length for file %s in directory %s exceeds maximum allowed length\n", entry->d_name, directory);
            continue;
        }
        if((snprintf(path, MAX_PATH_LEN, "%s/%s", directory, entry->d_name)) > MAX_PATH_LEN){
            print_error("Errore nella creazione del path per il file %s\n", entry->d_name);
            continue;
        }
        if(stat(path, &file_stat) != 0){
            print_error("Errore nella lettura delle informazioni del file %s\n", path);
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

void clear_all(linkedlist argfiles){
    delete_list(argfiles);

}


