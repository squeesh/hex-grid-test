

g++ -c -w -fPIC -IC:\python27\include -LC:\python27\libs -lpython27 src/*.cpp

g++ -shared -w src/extern/all_externs.cpp *.o C:\MinGW\lib\libopengl32.a C:\MinGW\lib\libglu32.a -IC:\python27\include -LC:\python27\libs -lpython27 -lglew32 -lfreeglut -o externs.so

g++ src/main.cpp externs.so C:\MinGW\lib\libopengl32.a C:\MinGW\lib\libglu32.a -IC:\python27\include -LC:\python27\libs -lglew32 -lfreeglut -lpython27 -o main.exe
