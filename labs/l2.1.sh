#!/bin/bash

mkdir pp

cd pp

for i in {1..50} 
do
mkdir fol$i 
cd fol$i 


for n  in {1..100} 
do
mkdir p$n
cd p$n 

for f in {1..200} 
do
touch file$f 
done 

cd ..
done 

cd ..
done 
