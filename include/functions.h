#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Functions for server setup
int parse_port(int argc, char *argv[]);
int create_socket();
int configure_and_bind_socket(int sock_fd, int port);
int start_listening(int sock_fd);
int initialise_server(int port);
void handle_client_connection(int client_fd, struct sockaddr_in client_addr);
void run_server(void);

// Functions to handle client thread
void* handle_client(void* arg);
int sender(int client_fd, const char *message);
void reciever(int client_fd);

#endif
