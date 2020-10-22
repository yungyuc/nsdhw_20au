#!/usr/bin/env bash

if [ "$#" -lt 1 ]; then
  echo "missing file name"
elif [ "$#" -gt 1 ]; then
  echo "only one argument is allowed"
elif [ ! -f "$1" ]; then
  echo "$1 not found"
else
  printf "%s lines in %s\n" $(wc -l countline.sh | cut -d ' ' -f 1) $1
fi
