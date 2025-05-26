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

// Create and return a socket file descriptor
int create_socket(void) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        printf("Couldn't create the socket... %s\n", strerror(errno));
        return -1;
    }
    return sock_fd;
}

// Configure and bind the socket to the specified port
int configure_and_bind_socket(int sock_fd, int port) {
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET, // IPv4 connections
        .sin_port = htons(port), // convert port from int to network byte address
        .sin_addr = { htonl(INADDR_ANY) }, // receive connections for all network interfaces
    };

    // Bind socket to the server address
    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        printf("Couldn't bind the socket to the server address... %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

// Start listening on the socket
int start_listening(int sock_fd) {
    int backlog = 5;
    if (listen(sock_fd, backlog) != 0) {
        printf("Couldn't mark the socket as listening... %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

// Initialise the server: create socket, bind, and start listening
int initialise_server(int port) {
    // Create the server socket
    server_fd = create_socket();
    if (server_fd < 0) {
        return -1;
    }

    // Configure and bind the socket
    if (configure_and_bind_socket(server_fd, port) != 0) {
        close(server_fd);
        return -1;
    }

    // Start listening
    if (start_listening(server_fd) != 0) {
        close(server_fd);
        return -1;
    }

    printf("Listening on port: %d\n", port);
    return 0;
}

// Handle a single client connection
void handle_client_connection(int client_fd, struct sockaddr_in client_addr) {
    // Allocate memory in heap for client information
    client_info_t* client_info = malloc(sizeof(client_info_t));
    if (client_info == NULL) {
        printf("Failed to allocate memory for client information...\n");
        close(client_fd);
        return;
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
        return;
    }

    // Detach the client thread for independence
    pthread_detach(thread_id);
}

// Main server loop for accepting client connections
void run_server(void) {
    while (1) {
        // Connect to the client
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_fd < 0) {
            printf("Couldn't connect to the client... %s\n", strerror(errno));
            continue;
        }

        // Handle the client connection
        handle_client_connection(client_fd, client_addr);
    }
}
