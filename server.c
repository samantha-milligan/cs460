#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/

 // Initialize mutex
 int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);

 int main(int argc, char** argv) {
     int server_socket;                          // descriptor of server socket
     struct sockaddr_in server_address;          // for naming the server's listening socket
     int client_socket;                          // number of clients we can accept

     pthread_t pthread_id[MAX_NUM_CONT_CLIENTS];

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
     //

     // listen for client connections (pending connections get put into a queue)
     if (listen(server_socket, NUM_CONNECTIONS) == -1) {
         perror("Error listening on socket");
         exit(EXIT_FAILURE);
     }

     // server loop
     while (TRUE) {
         // Lock mutex
         int pthread_mutex_lock(pthread_mutex_t *mutex);

         // accept connection to client and send handling to pthread
         if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
             perror("Error accepting client");
         } else {

             pthread_create(&pthread_id, NULL, handle_client, (void*)&client_socket);

             pthread_detach(pthread_id);
         }
     }
 }


/************************************************************************
 * handle client
 ************************************************************************/

void *handle_client(void *pthreaded_client_socket) {
    int* client_socket_ptr = (int*) pthreaded_client_socket;
    int client_socket = *((int*) pthreaded_client_socket);

    char input[150];
    int keep_going = TRUE;
    int close_val = 0;

    char *token;
    char *operator;
    char *input_1;
    char *input_2;
    char *output;

    while (keep_going) {
        // read char from client
        switch (read(client_socket, &input, sizeof(input))) {
            case 0:
                keep_going = FALSE;
                printf("\nEnd of stream, returning ...\n");
            case -1:
                perror("Error reading from network!\n");
                keep_going = FALSE;
                break;
        }

        printf("%s\n", input);

        // check if we terminate
        if (input[0] == 'q') {
            keep_going = FALSE;
        }
        else {
          token = strtok(input, ",");
          operator = token;

          token = strtok(NULL, ",");
          input_1 = token;

          token = strtok(NULL, ",");
          input_2 = token;

          printf("operator: %s\n", operator);
          printf("input 1: %s\n", input_1);
          printf("input 2: %s\n", input_2);
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


void comp_protocol(char *values[], char *response[]) {
  char *sqrt_detect = strstr(values[0], "sqrt");
  char *operator = values[0];
  int integer1 = atoi(values[1]);
  int integer2;
  char result_str[9999];
  int result;
  double double_result;

  response[0] = "OK";

  if(sqrt_detect) {
    printf("sqrt(%d)\n", integer1);

    if(integer1 < 0) {
      response[0] = "Negative Square Root";
      double_result = -9999.0;
    }
    else {
      double_result = sqrt(integer1);
    }

    sprintf(result_str, "%f", double_result);
    printf("Result: %f\n", double_result);

    response[1] = result_str;
  }
  else {
    integer2 = atoi(values[2]);
    printf("%d %s %d\n", integer1, operator, integer2);
    char single_operator = (char)*operator;

    switch(single_operator) {
      case '+':
        result = integer1 + integer2;
        break;
      case '-':
        result = integer1 - integer2;
        break;
      case '*':
        result = integer1 * integer2;
        break;
      case '/':
        if(integer2 == 0) {
          response[0] = "Division by Zero";
          double_result = -9999.0;
        }
        else {
          double_result = (double)integer1 / integer2;
        }
        break;
      case '^':
        result = pow(integer1, integer2);
        break;
    }

    if(single_operator == '/') {
      sprintf(result_str, "%f", double_result);
      printf("Result: %f\n", double_result);
    }
    else {
      sprintf(result_str, "%d", result);
      printf("Result: %d\n", result);
    }

    response[1] = result_str;
  }
}
