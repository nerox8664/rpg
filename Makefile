CC = gcc
CXX = g++

INCPATH =  -I/usr/include -I./Core/ -I.
CXXFLAGS =  -march=native -mtune=native -std=c++11 -O2

LIBS = -lGL -lGLU -lGLEW `sdl2-config --libs` -lSDL2_mixer
LIBS += -lSDL2_ttf -lSDL2_net -lIL -lILU -lsqlite3 -llua -lfreetype

SRC_C_CLIENT=$(wildcard Core/*.c)
SRC_CPP_CLIENT=$(wildcard Core/*.cpp)

OBJ_CLIENT=$(SRC_C_CLIENT:.c=.o) $(SRC_CPP_CLIENT:.cpp=.o)

all: $(OBJ_CLIENT)
	g++  $(OBJ_CLIENT) $(INCPATH) $(LIBS) $(CXXFLAGS) main.cpp -o bin/main

%.o: %.cpp
	g++ $(CXXFLAGS) $(INCPATH)  $^ -o $@ -c

clean:
	find . -name "*.o" -type f -delete

