CC = clang
CXX = clang++

INCPATH =  -I/usr/include -I./Core/ -I./Elements/ -I. -DDEBUG
CXXFLAGS =  -march=native -mtune=native -std=c++11 -Ofast

LIBS = -lGL -lGLU -lGLEW `sdl2-config --libs` -lSDL2_mixer -lSDL2_image
LIBS += -lSDL2_ttf -lSDL2_net -lIL -lILU -lsqlite3 -llua -lfreetype

SRC_C_CLIENT=$(wildcard Core/*.c) \
			 $(wildcard Core/Modules/*.c) \
			 $(wildcard Core/Components/*.c) \
			 $(wildcard Elements/*.c)

SRC_CPP_CLIENT=$(wildcard Core/*.cpp) \
			 $(wildcard Core/Modules/*.cpp) \
			 $(wildcard Core/Components/*.cpp) \
			 $(wildcard Elements/*.cpp)

OBJ_CLIENT=$(SRC_C_CLIENT:.c=.o) $(SRC_CPP_CLIENT:.cpp=.o)

all: $(OBJ_CLIENT)
	clang++  $(OBJ_CLIENT) $(INCPATH) $(LIBS) $(CXXFLAGS) main.cpp -o bin/main

%.o: %.cpp
	clang++ $(CXXFLAGS) $(INCPATH)  $^ -o $@ -c

clean:
	find . -name "*.o" -type f -delete
