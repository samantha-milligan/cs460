#include "server.h"

// Sends mathematical problem to server, returns answer
int main() {
    int socket_fd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_length = sizeof(server_addr);
    ssize_t bytes_received;
    char receive_buffer[MAX_LENGTH_DATA];
    char *client_message;

    // Creating client datagram socket
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created.\n\n");
    
    // Prepare server adddress structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_addr.sin_port = htons(PORT);

    // Display initial messages
    printf("Computation Client\n");
    printf("Type 'quit' to close socket.\n");
    printf("Use sqrt() for square root.\n\n");
    char user_input[100], message[9999], end_marker[] = "quit";
    char values[100];

    // While socket is connected, read message
    while (TRUE) {
        // Ask user for operation
        printf("Input: ");
        scanf("%s", user_input);

        // Check if close socket
        if (strcmp(user_input, end_marker) == 0) {
            // Write quit message to server
            sendto(
                socket_fd,
                (const char *)end_marker,
                strlen(end_marker) + 1,
                0,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)
            );

            // Close socket
            close(socket_fd);
            printf("\nSocket closed.\n\n");
            exit(EXIT_SUCCESS);
        }

        // Separate integers and operators
        separate_operators(user_input, values);

        // Send values to server
        sendto(
            socket_fd,
            (const char *)values,
            strlen(values) + 1,
            0,
            (struct sockaddr *) &server_addr,
            sizeof(server_addr)
        );

        // Receive message back from server
        bytes_received = recvfrom(
            socket_fd,
            (void *)receive_buffer,
            (size_t) MAX_LENGTH_DATA,
            MSG_WAITALL,
            (struct sockaddr *) &server_addr,
            &server_addr_length
        );

        // Terminate string received
        receive_buffer[bytes_received] = '\0';
        
        printf("Output: %s\n", receive_buffer);
    }

    return EXIT_SUCCESS;
}

// Separate operations for server
char* separate_operators(char* input, char values[]){
    // Find operator
    int index = 0;
    char operator_list[] = {'+', '-', '*', '/', '^', '('};
    char *found, operator;

    for(index = 0; index < 6; index++){
        found = strchr(input, operator_list[index]);
        if(found != NULL){
            operator = *found;
        }
    }

    // Add operator to values
    strcpy(values, "");
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

    // Remove last character for sqrt()
    char *remove_op = ")";
    char *second_par = strstr(substring, remove_op);
    if(second_par){
        strtok(values, remove_op);
    }
    
    return values;
}
