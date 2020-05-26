CC = gcc
CFLAG = -Wall -c
TERM = gnome-terminal -e
SERV = server
CLIENT1 = clientType1
CLIENT2 = clientType2
ADDFILE = Msg

all: $(SERV) $(CLIENT1) $(CLIENT2)

$(SERV): $(SERV).c $(ADDFILE).o struct.h
	$(CC) $(SERV).c $(CFLAG) -o $(SERV).o
	$(CC) $(SERV).o $(ADDFILE).o -o $(SERV)

$(CLIENT1): $(CLIENT1).c $(ADDFILE).o struct.h
	$(CC) $(CLIENT1).c $(CFLAG) -o $(CLIENT1).o
	$(CC) $(CLIENT1).o $(ADDFILE).o -o $(CLIENT1)

$(CLIENT2): $(CLIENT2).c $(ADDFILE).o struct.h
	$(CC) $(CLIENT2).c $(CFLAG) -o $(CLIENT2).o
	$(CC) $(CLIENT2).o $(ADDFILE).o -o $(CLIENT2)

$(ADDFILE).o: $(ADDFILE).c struct.h
	$(CC) $(ADDFILE).c $(CFLAG) -o $(ADDFILE).o

test:
	$(TERM) "${PWD}/$(CLIENT1)"
	$(TERM) "${PWD}/$(CLIENT1)"
	$(TERM) "${PWD}/$(CLIENT1)"
	$(TERM) "${PWD}/$(CLIENT2)"
	$(TERM) "${PWD}/$(CLIENT2)"
	$(TERM) "${PWD}/$(CLIENT2)"
	$(TERM) "${PWD}/$(SERV)"

clean:
	rm $(SERV) $(CLIENT1) $(CLIENT2) $(SERV).o $(CLIENT1).o $(CLIENT2).o $(ADDFILE).o