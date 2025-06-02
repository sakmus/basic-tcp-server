#include "../include/headers.h"
#include "../include/functions.h"


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

// Handle a single client
void* handle_client(void* arg) {
    // Extract client information
    client_info_t* client_info = (client_info_t*)arg;
    int client_fd = client_info->client_fd;
    struct sockaddr_in client_addr = client_info->client_addr;

    // Display client basic information
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Recieve data from client
    reciever(client_fd);

    // Send HTTP/1.1 200 OK response
    const char* http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><body><h1>HTTP 200 OK</h1><p>Your request was successfully recieved! The connection would be closed after this message.</p></body></html>";

    int status = sender(client_fd, http_response);
    if (status != 0) {
	printf("Couldn't send the message... %s\n", strerror(status));
    }

    // Gracefully close the connection and free up the memory
    close(client_fd);
    free(client_info);
    return NULL;
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
