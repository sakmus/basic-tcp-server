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

    // Run the main server
    run_server();

    // Close the server even though it won't come here due to infinite loop
    close(server_fd);

    return 0;
}
