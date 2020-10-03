#!/bin/bash

FILE=$1

if [ "$#" -lt 1 ]; then
	echo "missing file name"
	exit
elif [ "$#" -ge 2 ]; then
	echo "only one argument is allowed"
	exit
fi

if [ -f "$FILE" ]; then
	LINES="$(wc -l < $FILE)"
	echo "$LINES lines in $FILE"
else
	echo "$FILE not found"
fi
