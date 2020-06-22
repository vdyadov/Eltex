Модуль выполняющий обмен ифнормацией с пространством пользователя через sys

Тест модуля:

~$ make
~$ sudo su
root# insmod sysmod.ko
root# echo "Hello, user!" > /sys/kernel/mysys/test
root# cat /sys/kernel/mysys/test
root# echo "New message!" > /sys/kernel/mysys/test 
root# cat /sys/kernel/mysys/test 
root# rmmod sysmode.ko
root# dmesg

Отчистка:

~$ make clean
