CC = gcc
FLAGS = -L. -l$(LIB)
FILE = calc
LIB = calc
LIB_O = lib$(LIB).so
HEAD = funcs.h
FIRST_F = plus
SECOND_F = minus
THIRD_F = multiply
FOURTH_F = divide
OUT = $(FIRST_F).o $(SECOND_F).o $(THIRD_F).o $(FOURTH_F).o

all: $(FILE) $(LIB_O)

$(FILE): $(LIB_O)
	$(CC) $(FILE).c -o $(FILE) $(FLAGS)

$(LIB_O): $(OUT)
	$(CC) -shared $(OUT) -o $(LIB_O)

$(FIRST_F).o: $(FIRST_F).c $(HEAD)
	$(CC) $(FIRST_F).c -c	

$(SECOND_F).o: $(SECOND_F).c $(HEAD)
	$(CC) $(SECOND_F).c -c	

$(THIRD_F).o: $(THIRD_F).c $(HEAD)
	$(CC) $(THIRD_F).c -c	

$(FOURTH_F).o: $(FOURTH_F).c $(HEAD)
	$(CC) $(FOURTH_F).c -c	

test:
	$ LD_LIBRARY_PATH=$(PWD):$(LD_LIBRARY_PATH) ./$(FILE)

clean:
	rm -rf $(FILE) $(FIRST_F).o $(SECOND_F).o $(THIRD_F).o \
	$(FOURTH_F).o $(LIB_O)
