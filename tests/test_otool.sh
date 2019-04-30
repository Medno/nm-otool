#!/bin/bash

export FILES=./corrupted_folder/*
export OPTIONS=("-t" "-d" "-h")
export MULTI_OPTIONS=("-t -d" "-t -h" "-d -h" "-t -d -h")

for opt in ${OPTIONS[@]}
do
	echo "otool $opt"
	for file in $FILES
	do
		echo $file
		diff <(../ft_otool $opt $file) <(otool $opt $file)
	done
done

echo "<----- Multi-opts tests ----->"
for opt in "${MULTI_OPTIONS[@]}"
do
	echo "otool $opt"
	for file in $FILES
	do
		echo $file
		diff <(../ft_otool $opt $file) <(otool $opt $file)
	done
done
