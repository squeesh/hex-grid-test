#!/bin/sh

g++ -c -w -fPIC -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython27 color.cpp vertex.cpp hexagon.cpp controller.cpp util.cpp 2>&1

g++ -shared -w -fPIC hexagon_extern.cpp color.o vertex.o hexagon.o controller.o util.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lpython2.7 -o hexagon.so 2>&1
g++ -shared -w -fPIC controller_extern.cpp color.o vertex.o hexagon.o controller.o util.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lpython2.7  -o controller.so 2>&1

g++ -w main.cpp color.o vertex.o hexagon.o controller.o util.o -I/usr/include/python2.7 -L/usr/lib/python2.7 -lGL -lGLU -lpython2.7 -lglut -o main.o 2>&1

