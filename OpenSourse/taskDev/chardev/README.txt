Модули ядра: devfs

Задание выполнил: Дядов Владислав

Написать модуль ядра, который будет обмениваться информацией с userspace через /dev/mydev

Тест модуля:

~$ make
~$ sudo su
root# insmod mydev.ko
root# mknod -m0666 /dev/test c 238 0
root# echo "New message!" > /dev/test
root# cat /dev/test
root# rm /dev/test
root# rmmod mydev.ko
root# dmesg

Отчистка:

~$ make clean
