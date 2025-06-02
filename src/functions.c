#include "../include/headers.h"
#include "../include/functions.h"

// Parse and validate port number from command line arguments
int parse_port(int argc, char *argv[]) {
    int port;
    if (argc == 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            printf("Invalid port number provided. Using default.\n");
            port = DEFAULT_PORT;
        }
    } else {
        port = DEFAULT_PORT;
        printf("No port provided. Using the default port %d\n", port);
    }
    return port;
}

// Create the socket and return file descriptor
int create_socket(void) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        printf("Couldn't create the socket... %s\n", strerror(errno));
        return -1;
    }
    return sock_fd;
}

// Recieve data from the other side
void reciever(int sock_fd) {
    char buffer[4096] = {0};
    ssize_t bytes_size = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_size > 0) {
	buffer[bytes_size] = '\0';
	printf("Recieved data:\n%s\n", buffer);
    }
    return;
}

// Send data to the other side
int sender(int sock_fd, const char *message) {
    int status = send(sock_fd, message, strlen(message), 0);
    if (status == -1) {
	return errno;
    }
    return 0;
}
