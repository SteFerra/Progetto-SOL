#include <stdio.h>
#include "util.h"

typedef struct result{
    long value;  //risultato
    char filepath[MAX_PATH_LEN + 1];  //nome del file
}result ;

typedef struct resultarray{
    result *buf;
    size_t size;
    size_t capacity;
}resultarray;


int init_resultarray(resultarray **array);

int init_resultarray(resultarray **array){
    if((*array = malloc (sizeof(resultarray))) == NULL){
        perror("malloc");
        return -1;
    }
    (*array)->size = 0;
    (*array)->capacity = 10;
    if(((*array)->buf = malloc((*array)->capacity * sizeof(result))) == NULL){
        perror("malloc");
        free(*array);
        return -1;
    }

    //printf("Array dei risultati inizializzato\n");
    return 0;
}

int add_result(resultarray *array, long value, char *filepath){
    if(array == NULL){
        return -1;
    }
    if(array->size == array->capacity){
        result *res;
        size_t new_capacity = array->capacity * 2;
        if((res = realloc(array->buf, new_capacity * sizeof(result))) == NULL){
            perror("realloc");
            return -1;
        }
        array->buf = res;
        array->capacity = new_capacity;
    }

    // Trova la posizione in cui inserire il nuovo risultato
    int i = 0;
    while(i < array->size && array->buf[i].value < value){
        i++;
    }

    // Sposta tutti i risultati successivi di una posizione verso l'alto
    for(int j = array->size; j > i; j--){
        array->buf[j] = array->buf[j-1];
    }

    // Inserisce il nuovo risultato nella posizione trovata
    array->buf[i].value = value;
    strncpy(array->buf[i].filepath, filepath, MAX_PATH_LEN);
    array->buf[i].filepath[MAX_PATH_LEN] = '\0';
    array->size++;

    return 0;
}

int print_result(resultarray *array){
    if(array == NULL){
        return -1;
    }
    for(int i = 0; i < array->size; i++){
        printf("%ld %s\n", array->buf[i].value, array->buf[i].filepath);
    }
    printf("\n");
    return 0;
}

int delete_resultarray(resultarray *array){
    if(array == NULL){
        return -1;
    }
    free(array->buf);
    free(array);
    return 0;
}