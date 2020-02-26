#!/bin/bash

find / > a.txt
wc -l a.txt > b.txt
echo "Всего файлов:" 
cat b.txt
rm a.txt
rm b.txt
