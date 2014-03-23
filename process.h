#ifndef PROCESS_H
#define PROCESS_H

#include "main.h"

extern int create_multiprocess(int proc_num, void (*func)(void));
extern void do_execute_wait();
extern void do_signal_sigint(int);
#endif
