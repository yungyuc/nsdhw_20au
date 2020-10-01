#!/bin/bash

if [ 0"$PYTHON_BIN" = "0" ] ; then
    exec "$PYTHON_BIN" 1>&2
    exit 1
else
    if [ "$PYTHON_BIN" == "python2" ] ; then
        #echo "2ok"
        python2 - "$@" <<END
#!/usr/bin/env python
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
#print("123")
END
    elif [ "$PYTHON_BIN" == "python3" ] ; then
        #echo "3ok"
        python3 - "$@" <<END
#!/usr/bin/env python
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
#print("456")
END
    else
        exec "$PYTHON_BIN" 1>&2
        exit 1
    fi
fi
