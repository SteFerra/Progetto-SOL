#ifndef PROGETTO_MY_SIGNAL_H
#define PROGETTO_MY_SIGNAL_H

#include <signal.h>
#include <pthread.h>


void set_signal_mask();
void register_handlers();
void termination_handler(int signum);
void unmask();

extern volatile sig_atomic_t interrupted;


#endif //PROGETTO_MY_SIGNAL_H
