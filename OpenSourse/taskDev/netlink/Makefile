CC=gcc
CFLAG= -Wall -o
obj-m += netlink.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) user.c $(CFLAG) user
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm user

test:
	sudo insmod netlink.ko
	sudo dmesg | tail -1
	./user
	sudo dmesg | tail -1
	sudo rmmod netlink.ko
	sudo dmesg | tail -1
