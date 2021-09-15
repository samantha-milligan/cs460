#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char** argv) {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket[5];              // number of clients we can accept

    int avail_socket;                  // use this socket new connections
    pthread_t pthread_id[5];

    // sent when, client disconnected
    signal(SIGPIPE, SIG_IGN);

    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(PORT);       // port to listen on

    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    // server loop
    while (TRUE) {
        // check which index in client_socket available
        for (avail_socket = 0; avail_socket < 5 ; avail_socket++) {
            if (client_socket[avail_socket] == 0 ) {
                break;
          }
        }

        // avail_socket didn't chage and client_socket[0] is busy
        if (client_socket[0] != 0 && avail_socket == 0) {
            continue;
        } // accept connection to client and send handling to pthread
        else if ((client_socket[avail_socket] = accept(server_socket, NULL, NULL)) == -1) {
            perror("Error accepting client");
        } else {
            printf("\nAccepted client\n");
            //pthread_create(&thread_ID, NULL, handle_client, (void*)client_socket + avail_socket)
            handle_client((void*)client_socket + avail_socket);
        }
    }
}


/************************************************************************
 * handle client
 ************************************************************************/

void *handle_client(void *pthreaded_client_socket) {
    int* client_socket_ptr = (int*)pthreaded_client_socket;
    int client_socket = *client_socket_ptr;

    char input;
    int keep_going = TRUE;
    int close_val = 0;

    while (keep_going) {
        // read char from client
        switch (read(client_socket, &input, sizeof(char))) {
            case 0:
                keep_going = FALSE;
                printf("\nEnd of stream, returning ...\n");
                break;
            case -1:
                perror("Error reading from network!\n");
                keep_going = FALSE;
                break;
        }

        printf("%c", input);

        // check if we terminate
        if (input == 'q') {
            keep_going = FALSE;
        }

        // send result back to client
        write(client_socket, &input, sizeof(char));

    }

    // cleanup

    close_val = close(client_socket);
    *client_socket_ptr = 0;           // change client_socket array in main

    if (close_val == -1) {
        perror("Error closing socket\n");
        exit(EXIT_FAILURE);
    } else {
        printf("\nClosed socket to client, exit\n");
    }
}
