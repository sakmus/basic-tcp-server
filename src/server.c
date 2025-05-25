#include "../include/headers.h"
#include "../include/functions.h"


// Gracefully close the server
int server_fd;
void handle_signal(int arg) {
    printf("Closing the server...\n");
    exit(0);
}


int main(int argc, char *argv[]) {
    // Handle signal
    signal(SIGINT, handle_signal);

    // Get the port number
    int port = parse_port(argc, argv);

    // Initialise the server
    if (initialise_server(port) != 0) {
	return 1;
    }

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
