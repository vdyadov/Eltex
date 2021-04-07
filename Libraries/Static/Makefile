CC = gcc
FLAGS = -L. -l$(LIB)
FILE = calc
LIB = calc
HEAD = funcs.h
FIRST_F = plus
SECOND_F = minus
THIRD_F = multiply
FOURTH_F = divide
OUT = $(FIRST_F).o $(SECOND_F).o $(THIRD_F).o $(FOURTH_F).o

all: $(FILE) 

$(FILE): lib$(LIB).a
	$(CC) $(FILE).c -o $(FILE) $(FLAGS)

lib$(LIB).a: $(OUT) $(HEAD)
	ar rc lib$(LIB).a $(OUT)

$(FIRST_F).o: $(FIRST_F).c
	$(CC) $(FIRST_F).c -c	

$(SECOND_F).o: $(SECOND_F).c
	$(CC) $(SECOND_F).c -c	

$(THIRD_F).o: $(THIRD_F).c
	$(CC) $(THIRD_F).c -c	

$(FOURTH).o: $(FOURTH_F).c
	$(CC) $(FOURTH_F).c -c	

test:
	./$(FILE)

clean:
	rm -rf $(FILE) $(FIRST_F).o $(SECOND_F).o $(THIRD_F).o \
	$(FOURTH_F).o lib$(LIB).a
