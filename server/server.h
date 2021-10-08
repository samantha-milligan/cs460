/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
void *handle_client(void *pthreaded_client_socket);
int three_a_plus_one(int input);
int three_a_plus_one_rec(int number);

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define SERVER_ADDR "127.0.0.1" // loopback ip address
#define PORT 23657              // port the server will listen on

// I cannot let go of the old-fashioned way :) - for readability ...
#define FALSE false
#define TRUE !false

// number of pending connections in the connection queue
#define NUM_CONNECTIONS 100
#define MAX_NUM_CONT_CLIENTS 2
