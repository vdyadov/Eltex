СС = gcc

all: run

run: main.o
	$(CC) -Wall -o run main.o
main.o: main.c
	$(CC) -Wall -c -o main.o main.c 

clean:
	rm run main.o