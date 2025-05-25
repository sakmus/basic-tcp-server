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

#ifndef HEADERS_H
#define HEADERS_H

typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
} client_info_t;

#endif