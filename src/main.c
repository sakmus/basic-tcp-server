#include "../include/headers.h"
#include "../include/functions.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PORT 4545

// Gracefully close the server
int server_fd;
void handle_signal(int arg) {
    printf("Closing the server...\n");
    exit(0);
}

// Structure to pass client information
typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
} client_info_t;

int main(int argc, char *argv[]) {
    // Handle signal
    signal(SIGINT, handle_signal);

    // Create the server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Couldn't create the socket... %s\n", strerror(errno));
        return 1;
    }

    // Get the port number
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

    // Configure server
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET, // IPv4 connections
        .sin_port = htons(port), // convert port from int to network byte address
        .sin_addr = { htonl(INADDR_ANY) }, // recieve connections for all network interfaces
    };

    // Bind socket to the server adress
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        printf("Couldn't bind the socket to the server address... %s", strerror(errno));
        return 1;
    }

    // Start listening
    int backlog = 5;
    if (listen(server_fd, backlog) != 0) {
        printf("Couldn't mark the socket as listening... %s\n", strerror(errno));
        return 1;
    }
    printf("Listening on port: %d\n", port);

    // Server main loop
    while (1) {
        // Connect to the client
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_fd < 0) {
            printf("Couldn't connect to the client... %s\n", strerror(errno));
            continue;
        }

        // Allocate memory in heap for client information
        client_info_t* client_info = malloc(sizeof(client_info_t));
        if (client_info == NULL) {
            printf("Failed to allocate memory for client information...\n");
            close(client_fd);
            continue;
        }

        // Store client information
        client_info->client_fd = client_fd;
        client_info->client_addr = client_addr;

        // Create a client thread
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_info) != 0) {
            printf("Couldn't create a thread for client... %s\n", strerror(errno));
            close(client_fd);
            free(client_info);
            continue;
        }

        // Detach the client thread for independence
        pthread_detach(thread_id);

    }

    close(server_fd);

    return 0;
}
