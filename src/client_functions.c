#include "../include/headers.h"
#include "../include/functions.h"

const char* parse_host(int argc, char* argv[]) {
    if (argc == 3) {
	return argv[1];
    }
    return DEFAULT_HOST;
}

int configure_and_connect_to_host(int sock_fd, const char* host, int port) {
    // Configure the host
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
	printf("Invalid or unsupported address... %s\n", strerror(errno));
	return -1;
    }

    if (connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
	printf("Couldn't connect to the server... %s\n", strerror(errno));
	return -1;
    }
    return 0;
}
