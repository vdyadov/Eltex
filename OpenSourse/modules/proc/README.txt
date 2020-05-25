Модуль выполняющий обмен ифнормацией с пространством пользователя через proc

Тест модуля:

~$ make
~$ sudo su
root# insmod mymod.ko
root# echo "Hello, user!" > /proc/mymod
root# cat /proc/mymod
root# echo "New message!" > /proc/mymod
root# cat /proc/mymod
root# rmmod mymode.ko
root# dmesg

Отчистка:

~$ make clean
