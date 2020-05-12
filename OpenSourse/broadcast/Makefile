CC = gcc
TERM = gnome-terminal -e

build: 
	$(CC) server.c Msg.c -Wall -o server
	$(CC) clientType1.c Msg.c -Wall -o client1
	$(CC) clientType2.c Msg.c -Wall -o client2

test:
	$(TERM) "${PWD}/client1"
	$(TERM) "${PWD}/client2"
	$(TERM) "${PWD}/server"

clean:
	rm server
	rm client1
	rm client2
