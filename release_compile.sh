#!/bin/sh

g++ -std=c++0x -c -w -fPIC -Ofast -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython27 src/*.cpp 2>&1

g++ -std=c++0x -shared -w -fPIC -Ofast src/extern/all_externs.cpp *.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut -o externs.so 2>&1

g++ -std=c++0x -w -Ofast src/main.cpp ./externs.so -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut -o main.o 2>&1

