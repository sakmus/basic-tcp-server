#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 4545
#define BACKLOG 10

extern int server_fd;

typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
} client_info_t;



#endif
