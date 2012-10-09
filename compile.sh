#!/bin/sh

g++ -c -w -fPIC -g -g3 -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython27 src/*.cpp 2>&1

g++ -shared -w -fPIC -g -g3 src/extern/controller_extern.cpp *.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -o controller.so 2>&1

g++ -w -g -g3 src/main.cpp ./controller.so -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut -o main.o 2>&1

