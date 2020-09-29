#!/bin/bash

if [ $# -eq 0 ]
    then
        echo "missing file name"
elif [ $# -ge 2 ]
    then
        echo "only one argument is allowed"
else
    if [ -f $1 ]
        then
            wc -l $1 | awk '{ print $1 }'
    else
        echo "$1 not found"
    fi
fi
