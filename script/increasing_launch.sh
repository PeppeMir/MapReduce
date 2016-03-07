#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "Parameters error: expected MIN_NUM_THREAD, MAX_NUM_THREAD, STEP_THREAD and TIMES_PER_LAUNCH"
    exit 1
fi

MIN_NUM_THREAD=$1
MAX_NUM_THREAD=$2
STEP_THREAD=$3
TIMES_PER_LAUNCH=$4

for (( NUM_THREAD=$MIN_NUM_THREAD; NUM_THREAD<=$MAX_NUM_THREAD; NUM_THREAD=$(($NUM_THREAD + $STEP_THREAD)) ))
do
	echo "Launching test with $NUM_THREAD thread(s) for $TIMES_PER_LAUNCH time(s) ..."
	./multiple_launch.sh $NUM_THREAD $TIMES_PER_LAUNCH
done
