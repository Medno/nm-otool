#!/bin/bash

export FILES=./corrupted_folder/*
export OPTIONS=("-U" "-g" "-n" "-p" "-r" "-u" "-j" "-A")

echo "nm without options"
for file in $FILES
do
	echo $file
	diff <(../ft_nm $file) <(nm $file)
done

for opt in ${OPTIONS[@]}
do
	echo "nm $opt"
	for file in $FILES
	do
		echo $file
		diff <(../ft_nm $opt $file) <(nm $opt $file)
	done
done
