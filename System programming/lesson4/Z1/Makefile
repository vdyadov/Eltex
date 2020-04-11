CC = gcc

all: run

run: main.c funcs.h fork.c
	$(CC) -Wall -o run main.c
	$(CC) -Wall -o fork fork.c	

clean:
	rm run fork