#include "client.h"
#include "threadpool.h"

int main() {

    // task counter
    int task_counter;

    // create threadpool with THREADS_IN_POOL threads
    threadpool pool = threadpool_create();


    for (task_counter=1; task_counter<=NUMBER_TASKS; task_counter++)
    {
        // in each loop, execute talk_to_server in a thread from the pool
        threadpool_add_task(pool, task_copy_arguments, talk_to_server, (void*)&task_counter);
    }

    // lame way to wait for everybody to get done
    // in a network server, this is not needed as the main thread keeps accepting connections
    sleep(THREADS_IN_POOL);

    exit(EXIT_SUCCESS);
}

void talk_to_server(void *number_ptr) {

    // Create socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define socket address
    struct sockaddr_in client_address;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);

    // If no connection, return fail
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Connection failed.\n");
        exit(EXIT_FAILURE);
    }

    // Integer to send to server
    int integer = *((int*)number_ptr);

    // Send integer to server
    write(client_socket, &integer, sizeof(int));

    // Read and print message
    int step_number;
    read(client_socket, &step_number, sizeof(int));

<<<<<<< HEAD
    sleep(8);
=======
    // New format printing Number ---> Steps
    printf("Number: %d ----> Steps: %c \n",integer,step_number);

    //500 millisecond delay before closing
    sleep(0.500);
>>>>>>> cccdcd4f006641889615dcc4a9a17572193847d2

    // Close connection
    close(client_socket);
}

// Prepare arguments for thread function
void *task_copy_arguments(void *args_in) {

    void *args_out;

    args_out = malloc(sizeof(int));
    *((int*)args_out) = *((int*)args_in);

    return args_out;
}
