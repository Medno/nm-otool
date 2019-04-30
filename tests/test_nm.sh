#!/bin/bash

export FILES=./corrupted_folder/*
export OPTIONS=("-U" "-g" "-n" "-p" "-r" "-u" "-j" "-A")
export MULTI_OPTIONS=("-U -u" "-U -g" "-U -n" "-U -p" "-U -r" "-U -j" "-U -A" "-g -n" "-g -p" "-g -r" "-g -u" "-g -j" "-g -A" "-n -p" "-n -r" "-n -u" "-n -j" "-n -A" "-p -r" "-p -u" "-p -j" "-p -A" "-r -u" "-r -j" "-r -A" "-u -j" "-u -A" "-j -A")

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

echo "nm with options"
for opt in "${MULTI_OPTIONS[@]}"
do
	echo "nm $opt"
	for file in $FILES
	do
		echo $file
		diff <(../ft_nm $opt $file) <(nm $opt $file)
	done
done
