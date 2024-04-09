#include "util.h"



  void print_error(const char * str, ...) {
    const char err[]="ERRORE: ";
    va_list argp;
    char * p=(char *)malloc(strlen(str)+strlen(err)+EXTRA_LEN_PRINT_ERROR);
    if (!p) {
        perror("malloc");
        fprintf(stderr,"FATAL ERROR nella funzione 'print_error'\n");
        return;
    }
    strcpy(p,err);
    strcpy(p+strlen(err), str);
    va_start(argp, str);
    vfprintf(stderr, p, argp);
    va_end(argp);
    free(p);
}

static inline int isNumber(const char* s, int * n) {
    if (s==NULL) return 1;
    if (strlen(s)==0) return 1;
    char* e = NULL;
    errno=0;
    long val = strtol(s, &e, 10);
    if (val < 0) return 1;  //è positivo
    if (errno == ERANGE) return 2;    // overflow/underflow
    if (e != NULL && *e == (char)0) {
        *n = val;
        return 0;   // successo
    }
    return 1;   // non e' un numero
}

int checkPosNum(const char* optarg, int* option, const char* error_message){
    if(isNumber(optarg, option) != 0){
        print_error(error_message);
        return -1;
    }
    return 0;
}