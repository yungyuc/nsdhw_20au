#!/bin/bash
result=$(wc -l $1 | tr -dc '0-9')
echo "$result lines in  $1"
