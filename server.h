/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

// threads
#include <pthread.h>

/************************************************************************
 * function prototype declarations
 ************************************************************************/
void *handle_client(char *receive_buffer, int socket_fd,
                    struct sockaddr client_addr, socklen_t client_addr_length);
char* separate_operators(char* input, char values[]);
void comp_protocol(char *values[], char *response[]);

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define SERVER_ADDR "127.0.0.1" // loopback ip address
#define PORT 23657              // port the server will listen on
#define MAX_LENGTH_DATA 1024

#define FALSE false
#define TRUE !false

#define udp_server_h
#define udp_client_h
