CC = gcc

server: server.h server.o
	$(CC) server.c -o server

client: server.h client.o
	$(CC) client.c -o client

clean:
	rm server server.o client client.o
