#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Parameters error: expected THREAD_NUM and TIMES_PER_LAUNCH"
    exit 1
fi

THREAD_NUM=$1
TIMES_PER_LAUNCH=$2
LOG_FILE=mul_launch_log_$1_$2.txt

cd ../Code/

for (( c=1; c<=$TIMES_PER_LAUNCH; c++ ))
do
	echo "Launch #$c:" 
   ./MR $THREAD_NUM 
   ERROR_CODE=$?
   	if [ $ERROR_CODE -ne 0 ]; then
		echo "./MR $THREAD_NUM: error $ERROR_CODE" >> $LOG_FILE
	fi
done
