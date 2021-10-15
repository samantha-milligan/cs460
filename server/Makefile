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
