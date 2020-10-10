#!/usr/bin/env bash
"true" '''\'
if [ "$PYTHON_BIN" = "python2" ] || [ "$PYTHON_BIN" = "python3" ];
then
    exec "$PYTHON_BIN" "$0" "$1"
else
    echo "exec: $PYTHON_BIN: not found"
    exit 1
fi
exit 0
'''

import os
import sys

pyver = os.getenv('PYTHON_BIN')

if pyver == "python2" or pyver == "python3":
    import sys
    import os.path

    if len(sys.argv) < 2:
        sys.stdout.write('missing file name\n')
    elif len(sys.argv) > 2:
        sys.stdout.write('only one argument is allowed\n')
    else:
        fname = sys.argv[1]
        if os.path.exists(fname):
            with open(fname) as fobj:
                lines = fobj.readlines()
            sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
        else:
            sys.stdout.write('{} not found\n'.format(fname))
else:
    sys.exit(1)


