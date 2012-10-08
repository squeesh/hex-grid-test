#!/bin/sh

g++ -c -w -fPIC -g -g3 -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython27 src/vertex.cpp src/hexagon.cpp src/gameboard_chunk.cpp src/gameboard.cpp src/controller.cpp src/util.cpp src/global_consts.cpp 2>&1

g++ -shared -w -fPIC -g -g3 src/controller_extern.cpp vertex.o hexagon.o gameboard_chunk.o gameboard.o controller.o util.o global_consts.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -o controller.so 2>&1

g++ -w -g -g3 src/main.cpp ./controller.so -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lGLEW -lpython2.7 -lglut -o main.o 2>&1

