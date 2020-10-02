#!/usr/bin/env python2
import os
import sys
import os.path

if os.environ['PYTHON_BIN'] not in ('python2', 'python3'):
    sys.exit('exec: {}: not found\n'.format(os.environ['PYTHON_BIN']))

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
