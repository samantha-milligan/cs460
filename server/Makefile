CC = gcc

threadpool: threadpool.h
	$(CC) -c threadpool.c

threadpool_server: threadpool.o
	$(CC) -o threadpool_server threadpool_server.c threadpool.o -lpthread

multithreaded_server: multithreaded_server.h
	$(CC) -o multithreaded_server multithreaded_server.c -pthread

single_server:
	$(CC) -o single_server single_server.c

clean:
	rm threadpool_server multithreaded_server single_server threadpool.o

# TAKE EMILY'S APPROACH
# step through code, seeing what you can do.



# gcc -c threadpool.c to make object file
# gcc -o client client.c threadpool.o -lpthread
# bind to threadpool.o and pthread lib

# Wireshark packet snippet
