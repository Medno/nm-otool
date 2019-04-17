#!/bin/zsh
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_otool $file) <(otool -t $file)
done;
