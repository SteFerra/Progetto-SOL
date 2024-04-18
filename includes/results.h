#ifndef PROGETTO_RESULTS_H
#define PROGETTO_RESULTS_H

#include "util.h"

typedef struct result{
    long value;  //risultato
    char filepath[MAX_PATH_LEN + 1];  //nome del file
}result;

typedef struct resultarray *resultarray;

int init_resultarray(resultarray *array);
int add_result(resultarray array, long value, char *filepath);
int print_result(resultarray array);
int delete_resultarray(resultarray array);

#endif //PROGETTO_RESULTS_H
