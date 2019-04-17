#!/bin/zsh
echo "Basic nm"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm $file) <(nm $file)
done;

echo "nm -U"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -U $file) <(nm -U $file)
done;

echo "nm -g"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -g $file) <(nm -g $file)
done;

echo "nm -n"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -n $file) <(nm -n $file)
done;

echo "nm -p"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -p $file) <(nm -p $file)
done;

echo "nm -r"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -r $file) <(nm -r $file)
done;

echo "nm -u"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -u $file) <(nm -u $file)
done;

echo "nm -j"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -j $file) <(nm -j $file)
done;

echo "nm -A"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -A $file) <(nm -A $file)
done;

echo "nm -Aj"
for file in ../corrupted_folder/hell/** ;
do ;
	echo $file
	diff <(./ft_nm -Aj $file) <(nm -Aj $file)
done;

