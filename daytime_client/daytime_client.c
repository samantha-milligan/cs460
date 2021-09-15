#include "server.h"

// Daytime Client
// Connects to NIST server and displays current UTC time

int main(){
	// Create socket
    int client_socket;  
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define socket address
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    client_address.sin_port = htons(PORT);
    
    // Connect to daytime server
    printf("\nWaiting for connection...\n");
    
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == 0){
    	printf("Daytime client:\n");

	    // Read server message until end marker
	    char output;
	    char end_marker = '*';

	    while(output != end_marker){
	    	output = read(client_socket, &output, sizeof(char));
	    	printf("%c", output);
	    }

	    // Close socket
	    close(client_socket);
	    printf("\nConnection closed.");
	    exit(EXIT_SUCCESS);

	    return EXIT_SUCCESS;
    }

    printf("Connection failed.\n\n");
    exit(EXIT_FAILURE);
}
