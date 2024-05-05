#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "linked_list.h"

#define LIST_SIZE 10

// inizializza la struttura che conterrà la lista dei file passati come argomento
int init_list(linkedlist *list){
    if(list == NULL){
        return -1;
    }
    list->dim = 0;
    list->size= LIST_SIZE; //10 di default
    list->elem_size = MAX_PATH_LEN + 1;
    list->string = malloc(sizeof(char[list->size][list->elem_size]));
    if(list->string == NULL){
        return -1;
    }
    return 0;
}

// de alloca la memoria occupata dalla lista dei file
int delete_list(linkedlist *list){
    if(list == NULL){
        return -1;
    }
    free(list->string);
    return 0;
}

// aggiunge un file alla lista, se l array è pieno raddoppio la sua dimensione
int add_file(linkedlist *list, char* file){
    char (*tostring)[][list->elem_size];
    if(list == NULL) return -1;
    if(list->dim == list->size){
        void *res;
        size_t new_size = list->size * 2;
        if ((res = realloc(list->string, sizeof(char[new_size][list->elem_size]))) == NULL){
            return -1;
        }
        list->string = res;
        list->size = new_size;
    }
    tostring = list->string;
    strncpy((*tostring)[list->dim], file, list->elem_size);
    (*tostring)[list->dim][list->elem_size - 1] = '\0'; // assicura che il path sia terminato da un carattere nullo
    if(strlen(file) > list->elem_size){ // se il path è più lungo della dimensione massima termina con un errore
        return -1;
    }
    list->dim++;
    return 0;
}

// restituisce l'ultimo file inserito nella lista copiandolo in file e lo rimuove
int get_file(linkedlist *list, char* file){
    if(list == NULL || list-> dim == 0) return -1;
    char (*tostring)[][list->elem_size] = list->string;
    strncpy(file, (*tostring)[--(list->dim)], list->elem_size);
    return 0;
}