CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

server: server.c
	$(CC) server.c -o server $(CFLAGS)

client: client.c
	$(CC) client.c -o client $(CFLAGS)

clean:
	rm -f server client
