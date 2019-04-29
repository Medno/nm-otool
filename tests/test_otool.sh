#!/bin/bash

export FILES=./corrupted_folder/*
export OPTIONS=("-t" "-d" "-h")
#export OPTIONS=("-h")

for opt in ${OPTIONS[@]}
do
	echo "otool $opt"
	for file in $FILES
	do
		echo $file
		diff <(../ft_otool $opt $file) <(otool $opt $file)
	done
done
