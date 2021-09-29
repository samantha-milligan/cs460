#include "daytime_server.h"

// Daytime Client
// Connects to NIST server and displays current UTC time

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
        printf("Daytime client:");
    }

    char input[] = "";
    char message;
    char end_marker = '*';
    
    // While connected, read message
    while (TRUE) {
        // Read message
        fgets(input, sizeof(input), stdin);
        
        // Store message
        read(client_socket, &message, sizeof(char));

        // Read message until end marker, then close connection
        if (message == end_marker) {
            printf("\n\nConnection closed.\n\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }

        // Print full message
        printf("%c", message);
    }

    return EXIT_SUCCESS;
}
