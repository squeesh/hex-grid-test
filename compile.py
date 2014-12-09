#!/usr/bin/env python

import os
import re
from subprocess import call
import sys

compiler_str = 'i686-w64-mingw32-g++'

# if sys.platform == 'win32':
# bin_tpl = '{} -std=c++0x -c -w -fPIC -IC:\\Python27\\include -LC:\\Python27\\libs -lpython27 {} {}'
# shr_tpl = '{} -std=c++0x -shared -w {} build/*.o F:\\mingw\\lib\\libopengl32.a F:\\mingw\\lib\\libglu32.a -IC:\\Python27\\include -LC:\\Python27\\libs -lglew32 -lfreeglut -lpython27 {}'
# exe_tpl = '{} -std=c++0x {} ./bin/all_externs.so F:\\mingw\\lib\\libopengl32.a F:\\mingw\\lib\\libglu32.a -IC:\\Python27\\include -LC:\\Python27\\libs -lglew32 -lfreeglut -lpython27 {}'

# file_tpl = '{}/{}'
# extra_args_tpl = '-o build/{}.o'
# else:
    # bin_tpl = '{} -std=c++0x -c  -w -fPIC -g -g3 -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython27 {} {}'
    # shr_tpl = '{} -std=c++0x -shared -w -fPIC -g -g3 {} build/*.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut {}'
    # exe_tpl = '{} -std=c++0x -w -g -g3 {} ./bin/all_externs.so -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut {}'

    # file_tpl = '{}/{}'
    # extra_args_tpl = '-o build/{}.o'

bin_tpl = '{} -std=c++0x -c -w -fPIC -fpermissive -I/cygdrive/f/Python27/include -L/cygdrive/f/Python27/libs -lpython27 {} {}'
shr_tpl = '{} -std=c++0x -shared -w {} build/*.o /cygdrive/f/mingw/lib/libopengl32.a /cygdrive/f/mingw/lib/libglu32.a -I/cygdrive/f/Python27/include -L/cygdrive/f/Python27/libs -lglew32 -lfreeglut -lpython27 {}'
exe_tpl = '{} -std=c++0x {} ./bin/all_externs.so /cygdrive/f/mingw/lib/libopengl32.a /cygdrive/f/mingw/lib/libglu32.a -I/cygdrive/f/Python27/include -L/cygdrive/f/Python27/libs -lglew32 -lfreeglut -lpython27 {}'

file_tpl = '{}/{}'
extra_args_tpl = '-o build/{}.o'

output_rename = {
    'src/extern/all_externs.cpp': '-o bin/all_externs.so',
    'src/main.cpp': '-o main.o',
}

comp_shr = ['src/extern/all_externs.cpp']
comp_exe = ['src/main.cpp']

skip_files = comp_shr + comp_exe

for root, dirs, files in os.walk('./src'):
    for curr_file in files:
        if curr_file.endswith('.cpp'):
            filename = file_tpl.format(root[2:], curr_file)

            if filename not in skip_files:
                extra_args = extra_args_tpl.format(curr_file[:-4])

                print bin_tpl.format(compiler_str, filename, extra_args)
                retcode = call(bin_tpl.format(compiler_str, filename, extra_args), shell=True)

                if retcode != 0:
                    exit(0)

print '------'
for filename in comp_shr:
    print filename
    extra_args = output_rename[filename]
    print shr_tpl.format(compiler_str, filename, extra_args)
    retcode =  call(shr_tpl.format(compiler_str, filename, extra_args), shell=True)

    if retcode != 0:
        exit(0)

print '------'
for filename in comp_exe:
    print filename
    extra_args = output_rename[filename]
    print exe_tpl.format(compiler_str, filename, extra_args)
    call(exe_tpl.format(compiler_str, filename, extra_args), shell=True)
