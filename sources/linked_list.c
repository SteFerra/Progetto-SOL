#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "linked_list.h"



#define LIST_SIZE 10


int init_list(linkedlist *list){
    if(list == NULL){
        return -1;
    }
    list->dim = 0;
    list->size= LIST_SIZE;
    list->elem_size = MAX_PATH_LEN + 1;
    list->string = malloc(sizeof(char[list->size][list->elem_size]));
    if(list->string == NULL){
        return -1;
    }
    return 0;
}

int delete_list(linkedlist *list){
    if(list == NULL){
        return -1;
    }
    free(list->string);
    //printf("Lista dei file eliminata\n");
    return 0;
}

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
    (*tostring)[list->dim][list->elem_size - 1] = '\0'; // Ensure null termination
    if(strlen(file) > list->elem_size){
        return -1;
    }
    list->dim++;
    return 0;
}

int get_file(linkedlist *list, char* file){
    if(list == NULL || list-> dim == 0) return -1;
    char (*tostring)[][list->elem_size] = list->string;
    strncpy(file, (*tostring)[--(list->dim)], list->elem_size);
    return 0;
}

/*
int print_list(linkedlist *list){
    if(list == NULL || list->dim == 0){
        return -1;
    }

    char (*tostring)[][list->elem_size];
    tostring = list->string;
    for(int i = 0; i < list->dim; i++){
        struct stat file_stat;
        if(stat((*tostring)[i], &file_stat) != 0){
            printf("Errore nella lettura delle informazioni del file %s\n", (*tostring)[i]);
            continue;
        }
        printf("File %d: %s, Dimensioni: %ld bytes\n", i, (*tostring)[i], file_stat.st_size);
    }
    return 0;
}
 */

