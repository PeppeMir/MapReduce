#!/bin/bash          

MIC=$1
COPYPATH=FileGen/generated/

ssh $MIC 'mkdir Code; cd Code; mkdir FileGen; cd FileGen; mkdir generated'

cd $COPYPATH
local_arr=($(ls))
remote_arr=($(ssh $MIC 'ls ./FileGen/generated/'))

echo "Copying files ${local_arr[@]} on $MIC ..."
for file1 in "${local_arr[@]}"
do
	SKIP=0
	for file2 in "${remote_arr[@]}"
	do
		if [ "$file1" == "$file2" ]; then
			echo "Skipped copy of file \"$file1\" because laredy existing on $MIC..."
			SKIP=1
			break
		fi
	done
	
	if [ "$SKIP" -eq 0 ]; then
		scp ./$file1 $MIC:$COPYPATH
		echo "File \"$file1\" successfully copied on $MIC ..."
	fi
done
