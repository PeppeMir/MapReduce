#!/bin/bash          

if [ "$#" -ne 2 ]; then
    echo "Missing or too much parameters: expected tw parameters: HEADER VALUES"
    exit 1
fi

HEADER=$1
VALUES=$2
REPODIR=report
FILENAME=repo_file.csv

if [ ! -d "$REPODIR" ]; then
	mkdir report
fi

cd $REPODIR

if [ ! -e "$FILENAME" ]; then
	echo "$HEADER" >> $FILENAME
fi

echo "$VALUES" >> $FILENAME
