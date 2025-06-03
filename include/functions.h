#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Common Functions
int parse_port(int argc, char *argv[]);
int create_socket();
int sender(int sock_fd, const char *message);
void reciever(int sock_fd);

// Server Functions
int configure_and_bind_socket(int sock_fd, int port);
int start_listening(int sock_fd);
int initialise_server(int port);
void handle_client_connection(int client_fd, struct sockaddr_in client_addr);
void* handle_client(void* arg);
void run_server(void);

// Client Functions
const char* parse_host(int argc, char* argv[]);
int configure_and_connect_to_host(int sock_fd, const char* host, int port);

#endif
