#!/bin/bash

d=''
echo -n "В какой день долден сработать будильник? (гггг-мм-дд)"
read d

h=''
echo -n "В какой час сработает будильник? (чч)"
read h

declare -i m=''
echo -n "Во сколько минут сработает будильник? (мм)"
read m

declare -i z=0
z=$m+1

echo "$d $h:$m"
echo "$z"

sleep 20

u=$(date +%s -d "$d $h:$m")

echo "$u"

w=''
echo "Укажите путь к файлу с музыкой: (/home/USERNAME/wakeupdude/alarm.sh)"
read w

echo "DISPLAY=:0
$m $h * * *  $w" |crontab -
