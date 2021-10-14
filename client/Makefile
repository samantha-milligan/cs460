CC = gcc

threadpool: threadpool.h
	$(CC) -c threadpool.c

client: threadpool.o
	$(CC) -o client client.c threadpool.o -lpthread

clean:
	rm client
