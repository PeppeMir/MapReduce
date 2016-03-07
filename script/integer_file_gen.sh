#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Parameters error: expected RANDOM_GEN in {y, n}, NUMBER and FILE_SIZE"
    exit 1
fi

RANDOM_GEN=$1
NUMBER=$2
FILE_SIZE=$3
FILE_NM=file.txt

cd ../Code/FileGen/generated/
rm $FILE_NM

for (( c=1; c<=$FILE_SIZE; c++ ))
do
	if [ "$RANDOM_GEN" == "y" ]; then
		echo "$(( ( RANDOM % $NUMBER )  + 1 ))" >> $FILE_NM
	else
		echo "$NUMBER" >> $FILE_NM
	fi
done
