#ifndef SERVER_H
#define SERVER_H

#include "main.h"
#include "process.h"
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define LISTEN_PORT 10334
extern void start_server(int srv_num);
#endif
