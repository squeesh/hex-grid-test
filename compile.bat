g++ -c -w -fPIC -IC:\python27\include -LC:\python27\libs -lpython27 src/color.cpp src/vertex.cpp src/hexagon.cpp src/controller.cpp src/util.cpp src/global_consts.cpp

g++ -shared -w src/controller_extern.cpp color.o vertex.o hexagon.o controller.o util.o global_consts.o -IC:\python27\include -LC:\python27\libs -lpython27 -lopengl32 -lglu32  -o controller.so

g++ src/main.cpp color.o vertex.o hexagon.o controller.o util.o global_consts.o -IC:\python27\include -LC:\python27\libs -lopengl32 -lglu32 -lfreeglut -lpython27 -o main.exe
