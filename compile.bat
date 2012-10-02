

g++ -c -w -fPIC -IC:\python27\include -LC:\python27\libs -lpython27 src/vertex.cpp src/hexagon.cpp src/gameboard.cpp src/gameboard_chunk.cpp src/controller.cpp src/util.cpp src/global_consts.cpp

g++ -shared -w src/controller_extern.cpp vertex.o hexagon.o gameboard.o gameboard_chunk.o controller.o util.o global_consts.o C:\MinGW\lib\libopengl32.a C:\MinGW\lib\libglu32.a -IC:\python27\include -LC:\python27\libs -lpython27 -lglew32 -lfreeglut -o controller.so

g++ src/main.cpp controller.so C:\MinGW\lib\libopengl32.a C:\MinGW\lib\libglu32.a -IC:\python27\include -LC:\python27\libs -lglew32 -lfreeglut -lpython27 -o main.exe
