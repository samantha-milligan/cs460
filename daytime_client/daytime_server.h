/************************************************************************
 * libraries
 ************************************************************************/
// Necessary, standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Socket libraries
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Input/output libraries
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

/************************************************************************
 * function prototype declarations
 ************************************************************************/
void handle_client(int client_socket);

/************************************************************************
 * preprocessor directives
 ************************************************************************/
// Define server address and port
#define SERVER_ADDR "132.163.97.6"
#define PORT 13

// Naming conventions
#define TRUE !false

// Number of pending connections in the connection queue
#define NUM_CONNECTIONS 1
