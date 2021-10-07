#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

// turn on debugging
#define DBG
#include "dbg.h"

// Function declarations
void talk_to_server(void *number_ptr);

// Define number of tasks
#define NUMBER_TASKS 100000

// Define server address and port
#define SERVER_ADDR "127.0.0.1"
#define PORT 23657
