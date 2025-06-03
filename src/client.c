#include "../include/headers.h"
#include "../include/functions.h"

int main(int argc, char* argv[]) {
    int port = parse_port(argc, argv);
    const char* host = parse_host(argc, argv);

    printf("The host is being used: %s and the port is %d\n", host, port);

    int client_fd = create_socket();
    int status = configure_and_connect_to_host(client_fd, host, port);
    if (status != 0) {
	return 1;
    }

    const char* message = "Hello from the client!!";
    sender(client_fd, message);
    reciever(client_fd);

    close(client_fd);
    return 0;
}
