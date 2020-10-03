#!/usr/bin/env python3

import sys
import os

restart = os.getenv('PYTHON_RESTART')

python_bin = os.getenv('PYTHON_BIN')

argment = ""
for i in range(len(sys.argv)):
    argment = argment + '%s ' %(sys.argv[i])

if python_bin == 'python3':
    if restart == None:
        os.environ['PYTHON_RESTART'] = '1'
        os.system('python3 %s' %(argment));
        quit(0)
elif python_bin == 'python2':
    if restart != None:
        os.environ['PYTHON_RESTART'] = '1'
        os.system('python2 %s' %(argment));
        quit(0)
else:
    print('exec: python0: not found')
    quit(1)

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
    os.environ['PYTHON_RESTART'] = '' 
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
    os.environ['PYTHON_RESTART'] = ''
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
        os.environ['PYTHON_RESTART'] = ''
    else:
        sys.stdout.write('{} not found\n'.format(fname))
        os.environ['PYTHON_RESTART'] = ''
