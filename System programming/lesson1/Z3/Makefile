
build:
	gcc -Wall -c -o func.o func.c
	gcc -o func func.o
	gcc -Wall -c -o main.o main.c
	gcc -o main main.o

test1:
	./main 1 5 5

test2:
	./main 2 "It's worked!"

test3:
	./main 3 5 25 18 65 2 3

clean:
	rm main main.o func func.o