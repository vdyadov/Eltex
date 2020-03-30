CC = gcc

all: run

run: main.o funcs.o
	$(CC) -Wall -o run main.o funcs.o
main.o: main.c Header.h
	$(CC) -Wall -c -o main.o main.c
funcs.o: funcs.c
	$(CC) -Wall -c -o funcs.o funcs.c

clean:
	rm run main.o funcs.o