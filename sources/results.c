#include <stdio.h>
#include <malloc.h>

typedef struct result{
    long value;  //risultato
    char *file;  //nome del file
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

    printf("Array dei risultati inizializzato\n");
    return 0;
}