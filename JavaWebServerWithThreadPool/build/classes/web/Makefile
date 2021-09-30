CC = gcc

server: server.h server.o
	$(CC) -pthread -o server server.c

client: server.h client.o
	$(CC) -o client client.c

clean:
	rm server server.o client client.o
