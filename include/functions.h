#ifndef FUNCTIONS_H
#define FUNCTIONS_H


// Functions to handle client thread
void* handle_client(void* arg);
int sender(int client_fd, const char *message);
void reciever(int client_fd);

#endif
