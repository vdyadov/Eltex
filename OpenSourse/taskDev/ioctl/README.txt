Модули ядра: ioctl

Задание выполнил: Дядов Владислав

Написать модуль ядра, который будет обмениваться информацией с userspace через ioctl

Тест модуля:

~$ make
~$ sudo su
root# insmod keyleds.ko
root# echo 3 > /sys/kernel/keyledctl/test
root# echo 4 > /sys/kernel/keyledctl/test 
root# echo 6 > /sys/kernel/keyledctl/test 
root# echo 1 > /sys/kernel/keyledctl/test 
root# rmmod keyleds.ko 
root# dmesg

Отчистка:

~$ make clean

