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
        printf("Client connected.");

    }

    // Display initial messages
    printf("Computation Client");
    printf("Type 'quit' to close connection.");
    printf("Use sqrt() for square root.");

    char message;
    
    // While connected, read message
    while (TRUE) {

        // Ask user for operation
        printf("Input: ");
        char user_input;
        scanf("%c", &user_input);

        // Check if quit connection
        check_closed_connection(user_input);

        // Separate integers and operators
        values = separate_operators(user_input);

        // Create packet
        packet = create_packet(values);

        // Send packet to server
        // TODO - determine size of packet
        write(client_socket, &packet, sizeof(int));

        // Read and store server message
        read(client_socket, &message, sizeof(char));

        // Print full message
        printf("Output: ")
        printf("%c", message);
    }

    return EXIT_SUCCESS;
}

// Checks for closed connection
void check_closed_connection(char user_input){
    char end_marker = 'quit';

    if (user_input == end_marker) {
            printf("\n\nConnection closed.\n\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }
}

// Separate operations for packet
// TODO
char* separate_operators(char user_input){
    // Find operator
    int index = 0;
    char operator_list = {'+', '-', '/', '^'}
    char *found;
    
    for(index = 0; index < 4; index++){
        found = strchr(user_input, operator_list[index])
        if(found != NULL){
            operator = (char)found;
        }
    }
    
    // TODO - split string to find integers

    char value_list[];
    sqrt = strstr(user_input, 'sqrt()');

    if(sqrt){
        value_list = {'sqrt()', integer1}
    }
    else{
        value_list = {operator, integer1, integer2}
    }

    return value_list;
}

// Creates UDP packet separating operators and integers
create_packet(char* values){
    // TODO - return type in function declaration
    // TODO - create packet structure
}
