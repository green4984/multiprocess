#ifndef PROCESS_H
#define PROCESS_H

#include "main.h"
#include <stropts.h>

struct proc_t {
	int fd; /* socketpair AF_INET pair */
	int flag; /* 1 mean busy */
};

extern int create_multiprocess(int proc_num, void (*func)(int fd));
extern void send_fd(int sock_fd, int send_fd);
extern int recv_fd(int sock_fd);
extern void close_multiprocess();
extern struct proc_t *get_sleep_process();

#endif
