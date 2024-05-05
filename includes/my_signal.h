#ifndef PROGETTO_MY_SIGNAL_H
#define PROGETTO_MY_SIGNAL_H

#include <signal.h>
#include <pthread.h>

void set_signal_mask(sigset_t *set);
void ignore_signals();

extern volatile sig_atomic_t siginterruption_received;
extern volatile sig_atomic_t sigusr1_received;
extern volatile sig_atomic_t sigusr2_received;

#endif //PROGETTO_MY_SIGNAL_H
