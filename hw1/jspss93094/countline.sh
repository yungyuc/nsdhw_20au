#!/bin/bash

if [ "$#" -lt 1 ]
then
    echo 'missing file name'
    printf "\n"
elif [ "$#" -gt 1 ]
then
    echo 'only one argument is allowed'
    printf "\n"
else
    fname=$1
    if [ -f "$fname" ]; then
        fobj=$fname
        exec < $fobj
        while read line
            do
                ((count++))
            done
        lines=$count
        echo "$count lines in $fobj"
        printf "\n"
    else
        echo "$fname not found"
        printf "\n"
    fi
fi
