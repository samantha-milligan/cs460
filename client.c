#include "server.h"


int main() {
    // Create socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define socket address
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
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

    // Display initial messages
    printf("Computation Client\n");
    printf("Type 'quit' to close connection.\n");
    printf("Use sqrt() for square root.\n\n");

    char user_input[100], message, end_marker[] = "quit";
    char* values;

    // While connected, read message
    while (TRUE) {
        // Ask user for operation
        printf("Input: ");
        scanf("%s", user_input);

        // Check if quit connection
        if (strcmp(user_input, end_marker) == 0) {
            // Write quit message to server
            write(client_socket, &end_marker, sizeof(char));

            // Close connection
            printf("\nConnection closed.\n\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }

        // Separate integers and operators
        values = separate_operators(user_input);

        // Send values to server
        write(client_socket, &values, sizeof(values));

        // Read and print server message
        read(client_socket, &message, sizeof(message));
        printf("Output: ");
        printf("%c\n", message);
    }

    return EXIT_SUCCESS;
}

// TODO - Separate operations for packet
char* separate_operators(char* input){
    // Find operator
    int index = 0;
    char operator_list[] = {'+', '-', '/', '^', '(', '*'};
    char *found, operator;

    for(index = 0; index < 6; index++){
        found = strchr(input, operator_list[index]);
        if(found != NULL){
            operator = *found;
        }
    }

    // Add operator to values
    char values[100];
    char *sqrt = strstr(input, "sqrt");

    if(!sqrt){
        strcpy(values, &operator);
        strcat(values, ",");
    }

    // Find integers
    char *substring = strtok(input, &operator);
    strcat(values, substring);
    strcat(values, ",");
    substring = strtok(NULL, &operator);
    strcat(values, substring);

    printf("%s\n", values);

    return values;
}
