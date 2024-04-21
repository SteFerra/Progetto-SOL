#ifndef PROGETTO_MY_SIGNAL_H
#define PROGETTO_MY_SIGNAL_H

#include <signal.h>
#include <pthread.h>


void set_signal_mask(sigset_t *set);
void ignore_signals();
void register_handlers();
void termination_handler(int signum);
void sigusr1_handler(int signum);
void unmask();

extern volatile sig_atomic_t siginterruption_received;
extern volatile sig_atomic_t sigusr1_received;
extern volatile sig_atomic_t sigusr2_received;


#endif //PROGETTO_MY_SIGNAL_H
