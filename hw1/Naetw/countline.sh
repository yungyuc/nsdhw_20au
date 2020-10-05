#!/usr/bin/env bash

if [[ $# -lt 1 ]]; then
	echo "missing file name"
elif [[ $# -gt 1 ]]; then
	echo "only one argument is allowed"
else
    FILENAME=$1
    if [ -f "$FILENAME" ]; then
        LINES=$(cut -d ' ' -f 1 <( wc -l "$FILENAME" ))
        echo "$LINES lines in $FILENAME"
    else
        echo "$FILENAME not found"
    fi
fi
