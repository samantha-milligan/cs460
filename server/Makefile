CC = gcc

threadpool: threadpool.h
	$(CC) -c threadpool.c

t_server: threadpool.o
	$(CC) -o t_server t_server.c threadpool.o -lpthread

m_server: m_server.h
	$(CC) -o m_server m_server.c -pthread

s_server:
	$(CC) -o s_server s_server.c

clean:
	rm t_server m_server s_server threadpool.o

# TAKE EMILY'S APPROACH
# step through code, seeing what you can do.

# gcc -c threadpool.c to make object file
# gcc -o client client.c threadpool.o -lpthread
# bind to threadpool.o and pthread lib

# Wireshark packet snippet
