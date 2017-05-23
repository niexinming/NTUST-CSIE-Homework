CC=gcc
CFLAGS=-Wall -Wextra -O2

all:
	$(CC) $(CFLAGS) -c shm.c
	$(CC) $(CFLAGS) -c client.c
	$(CC) $(CFLAGS) -c server.c
	$(CC) $(CFLAGS) shm.o client.o -o client
	$(CC) $(CFLAGS) shm.o server.o -o server

clean:
	rm server
	rm client
