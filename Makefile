CC = gcc

client: server.h client.c
	$(CC) -o client client.c

server: server.h server.c
	$(CC) -o server server.c -lm
