#include "client.h"

// Client
// Sends user integer to Java 3A+1 server
// Prints algorithm steps from server

int main() {
    // Create socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define socket address
    struct sockaddr_in client_address;
    char* time_server_ip = inet_ntoa(*(struct in_addr *)(gethostbyname(SERVER_ADDR)->h_addr_list[0]));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(time_server_ip);
    client_address.sin_port = htons(PORT);

    // Connect to daytime server
    printf("\nWaiting for connection...\n\n");

    // If no connection, return fail
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Connection failed.\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Client connected.\n\n");
    }

    // Ask user for integer
    int integer;
    printf("Integer: ");
    scanf("%d", &integer);

    // Send integer to server
    write(client_socket, &integer, sizeof(int));

    // Read and print message
    printf("Steps: ");
    char message;
    read(client_socket, &message, sizeof(char));
    printf("%c", message);

    // Close connection
    printf("\n\nConnection closed.\n\n");
    close(client_socket);
    exit(EXIT_SUCCESS);
}
