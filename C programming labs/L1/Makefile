CC=gcc

all: run

run: encrypt.o
	$(CC) -o run encrypt.o
encrypt.o: encrypt.c
	$(CC) -c -o encrypt.o encrypt.c

clean:
	rm run encrypt encrypt.o