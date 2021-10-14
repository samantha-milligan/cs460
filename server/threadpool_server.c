#include "threadpool_server.h"
#include "threadpool.h"

int main(int argc, char** argv) {
    int server_socket;                          // descriptor of server socket
    struct sockaddr_in server_address;          // for naming the server's listening socket
    int client_socket;    // number of clients we can accept

    int avail_socket;                           // use this socket new connections
    pthread_t pthread_id[MAX_NUM_CONT_CLIENTS];
    int loop_for_avail;

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

    // create threadpool with THREADS_IN_POOL threads
    threadpool pool = threadpool_create();

    // server loop
    while (TRUE) {

        if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
            perror("Error accepting client");
        } else {
            threadpool_add_task(pool, task_copy_arguments, handle_client, (void*)&client_socket);
        }
    }
}

void *handle_client(void *pthreaded_client_socket) {
    int* client_socket_ptr = (int*) pthreaded_client_socket;
    int client_socket = *((int*) pthreaded_client_socket);

    int integer, step_number;

    // Read from client
    read(client_socket, &integer, sizeof(int));

    // Compute algorithm steps
    step_number = three_a_plus_one_rec(integer);

    // send result back to client
    write(client_socket, &step_number, sizeof(int));

    // output the inputted number and the steps
    printf("\n%d   ------->   ", integer);
    printf("%d\n", step_number);

    // mandated .5 second delay
    //usleep(500);

    // cleanup
    close(client_socket);

    return;
}

// Non-recursive 3A+1 algorithm
int three_a_plus_one(int input)
{
    int counter = 0;
    int current = input;

    while (current != 1)
    {
        counter++;
        if (current % 2) {
            current = (current*3) + 1;
        }
        else {
            current >>= 1;
        }
    }
    return counter;
}


// Recursive 3A+1 algorithm
int three_a_plus_one_rec(int number) {
    int new_number;

    if (number == 1) {
        return 0;
    }

    if (number % 2) {
        new_number = 3 * number + 1;
    } else {
        new_number = number / 2;
    }

    return 1 + three_a_plus_one(new_number);
}

// Prepare arguments for thread function
void *task_copy_arguments(void *args_in) {

    void *args_out;

    args_out = malloc(sizeof(int));
    *((int*)args_out) = *((int*)args_in);

    return args_out;
}
