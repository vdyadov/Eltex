CC=gcc

all: run

run: main.o sort.o
	$(CC) -o run main.o sort.o
main.o: main.c Header.h
	$(CC) -c -o main.o main.c
sort.o: sort.c
	$(CC) -c -o sort.o sort.c
