#ifndef SERVER_H
#define SERVER_H

#include "main.h"
#include "process.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define LISTEN_PORT 1334
extern void start_server();
#endif
