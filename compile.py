#!/usr/bin/env python

import os
import re
from subprocess import call
import sys

# ignore_dirs = ['./.git']

bin_tpl = 'g++ -std=c++0x -c -w -fPIC -g -g3 -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython27 {} {}'
shr_tpl = 'g++ -std=c++0x -shared -w -fPIC -g -g3 {} *.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut {}'
exe_tpl = 'g++ -std=c++0x -w -g -g3 {} ./all_externs.so -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut {}'

output_rename = {
    'src/extern/all_externs.cpp': '-o all_externs.so',
    'src/main.cpp': '-o main.o',
}

comp_shr = ['src/extern/all_externs.cpp']
comp_exe = ['src/main.cpp']

skip_files = comp_shr + comp_exe

for root, dirs, files in os.walk('./src'):
    for curr_file in files:
        if curr_file.endswith('.cpp'):
            filename = '{}/{}'.format(root[2:], curr_file)

            if filename not in skip_files:
                extra_args = '-o {}.o'.format(curr_file[:-4])

                print filename
                retcode = call(bin_tpl.format(filename, extra_args), shell=True)

                if retcode != 0:
                    exit(0)

                # retcode = call("mycmd" + " myarg", shell=True)
                # if retcode < 0:
                #     print >>sys.stderr, "Child was terminated by signal", -retcode

print '------'
for filename in comp_shr:
    print filename
    extra_args = output_rename[filename]
    print shr_tpl.format(filename, extra_args)
    retcode =  call(shr_tpl.format(filename, extra_args), shell=True)

    if retcode != 0:
        exit(0)

print '------'
for filename in comp_exe:
    print filename
    extra_args = output_rename[filename]
    print exe_tpl.format(filename, extra_args)
    call(exe_tpl.format(filename, extra_args), shell=True)
