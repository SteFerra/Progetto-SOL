#ifndef PROGETTO_UTIL_H
#define PROGETTO_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PATH_LEN 255

#if !defined(EXTRA_LEN_PRINT_ERROR)
#define EXTRA_LEN_PRINT_ERROR   512
#endif

#define PRINT_HELP printf("Utilizza: ./farm -n <nthread> -q <qlen> -t <delay> [-d <directory-name>] e/o [list-of-files] \n");

void print_error(const char * str, ...);
int checkPosNum(const char* optarg, int* option, const char* error_message);
int isNumber(const char* s, int * n);
ssize_t writen(int fd, void *buf, size_t size);

#endif //PROGETTO_UTIL_H
