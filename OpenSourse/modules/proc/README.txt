Модуль выполняющий обмен ифнормацией с пространством пользователя через proc

Тест модуля:

$ make
$ sudo su
# insmod mymod.ko
# echo "Hi, user!" > /proc/mymod
# cat /proc/mymod
# echo "New line!" > /proc/mymod
# cat /proc/mymod
# rmmod mymode.ko
# dmesg

Отчистка:

$ make clean
