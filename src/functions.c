#include "../include/headers.h"
#include "../include/functions.h"

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

void reciever(int client_fd) {
    char buffer[4096] = {0};
    ssize_t bytes_size = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_size > 0) {
	buffer[bytes_size] = '\0';
	printf("Recieved data:\n%s\n", buffer);
    }
    return;
}

int sender(int client_fd, const char *message) {
    int status = send(client_fd, message, strlen(message), 0);
    if (status == -1) {
	return errno;
    }
    return 0;
}
