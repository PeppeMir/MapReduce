#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Parameters error: expected TIMES_PER_LAUNCH"
    exit 1
fi

TIMES_PER_LAUNCH=$1
NUM_THREADS=(1 2 4 8 10 16 20 25 32 40 45 58 59 60 61 62 65 70 74 80 83 90 97 100 106 111 118 119 120 121 122 128 135 144 158 170 178 179 180 181 182 194 202 215 222 234 238 239 240)

for val in "${NUM_THREADS[@]}"
do
	echo "Launching test with $val thread(s) for $TIMES_PER_LAUNCH time(s) ..."
	./multiple_launch.sh $val $TIMES_PER_LAUNCH
done
