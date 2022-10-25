CC=gcc
CFLAGS=-W -Wall -g

all: server client

server:
	${CC} ${CFLAGS} parse.c server.c -o server -lpthread -ljson-c

client:
	${CC} ${CFLAGS} client.c -o client

clean:
	rm -f *.o server client
