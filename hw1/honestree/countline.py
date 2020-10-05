#!/bin/bash

eval $PYTHON_BIN << EOL
import sys
import os.path

fname = "$1"
if os.path.exists(fname):
    with open(fname) as fobj:
        lines = fobj.readlines()
    sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
else:
    sys.stdout.write('{} not found\n'.format(fname))
EOL

if [ $? -ne 0 ]
  then
  echo "exec: $PYTHON_BIN: not found"
    exit 1
  else
    exit 0
fi

