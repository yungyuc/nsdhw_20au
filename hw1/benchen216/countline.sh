#!/usr/bin/env bash
if [ $# -lt  1 ]
then
  echo "missing file name"
elif [ $# -gt  1 ]
then
    echo "only one argument is allowed"
else
  # shellcheck disable=SC2006
  a=`wc -l "$1" | awk '{print $1}'`
  echo "$a lines in $1"
fi