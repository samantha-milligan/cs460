#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/

 int main(int argc, char** argv) {
     int                socket_fd;                                // descriptor of server socket
     struct sockaddr_in server_addr;                              // for naming the server's listening socket
     struct sockaddr    client_addr;
     socklen_t          client_addr_length = sizeof(client_addr); // number of clients we can accept

     ssize_t            bytes_received;

     char               receive_buffer[MAX_LENGTH_DATA];
     char               *server_msg = "10";

     // creating datagram socket file descriptor
     if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
     {
         perror("Socket creation failed");
         exit(EXIT_FAILURE);
     }

     // name the socket (making sure the correct network byte ordering is observed)
     memset(&server_addr, 0, sizeof(server_addr));
     server_addr.sin_family      = AF_INET;           // accept IP addresses
     server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
     server_addr.sin_port        = htons(PORT);       // port to listen on

     // binding unnamed socket to a particular port
     if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
         perror("Error binding socket");
         exit(EXIT_FAILURE);
     }

     // server loop
     while (TRUE) {
         // receive message from *any* client
         // data of up to MAX_LENGTH_DATA will be put into buffer
         // client_addr will be filled in with addr info of where the data was received from
         if ((bytes_received = recvfrom(socket_fd,
                                   (void *)receive_buffer,
                                   (size_t) MAX_LENGTH_DATA,
                                   MSG_WAITALL,
                                   (struct sockaddr *) &client_addr,
                                   &client_addr_length
                                   )) == -1)
         {
             perror("recvfrom failed");
         } else {

           // terminate string
           receive_buffer[bytes_received] = '\0';

           handle_client(receive_buffer, socket_fd, client_addr, client_addr_length);
         }
     }
 }


/************************************************************************
 * handle client
 ************************************************************************/

void *handle_client(char *receive_buffer, int socket_fd,
                    struct sockaddr client_addr, socklen_t client_addr_length) {

    char *token;
    char operator_char;

    char *operator;
    char *input_1;
    char *input_2;
    char *output;

    token = strtok(receive_buffer, ",");
    operator = token;

    token = strtok(NULL, ",");
    input_1 = token;

    token = strtok(NULL, ",");
    input_2 = token;

    char *sqrt_detect = strstr(operator, "sqrt");

    if(sqrt_detect){
      printf("Input: %s(%s)\n", operator, input_1);
    } else {
      operator_char = (char) *operator;
      printf("Input: %s%c%s\n", input_1, operator_char, input_2);
    }

    char *values_list[3] = {operator, input_1, input_2};

    char *response[2] = {"", ""};

    comp_protocol(values_list, response);

    char *error_str = strstr(response[0], "Error");
    char message[9999];

    if(error_str){
      strcpy(message, response[0]);
    }
    else {
      strcpy(message, response[1]);
    }

    // send message back to client
    // data is contained in data
    // the client addr info is in client_addr, which was filled in in recvfrom() call before
    if(sendto(socket_fd,
           (const char *)message,
           strlen(message) + 1, // account for ASCII?
           0,
           (struct sockaddr *) &client_addr,
           client_addr_length) == -1)
    {
        perror("sendto");
    } else {
        printf("Output: %s\n", message);
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
    if(integer1 < 0) {
      response[0] = "Error: Negative Square Root";
      double_result = -9999.0;
    }
    else {
      double_result = sqrt(integer1);
    }

    sprintf(result_str, "%f", double_result);

    response[1] = result_str;
  }
  else {
    integer2 = atoi(values[2]);
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
          response[0] = "Error: Division by Zero";
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
    }
    else {
      sprintf(result_str, "%d", result);
    }

    response[1] = result_str;
  }
}
