CC=g++
CFLAGS=-O3 -Wall `sdl-config --cflags`
LIBS=`sdl-config --libs` -lm -lSDL_ttf -lSDL_image

all: pacman

pacman: main.cpp graphics.o input.o
	$(CC) $(CFLAGS) main.cpp graphics.o input.o -o pacman $(LIBS)

graphics.o: graphics.cpp graphics.hpp
	$(CC) $(CFLAGS) -c graphics.cpp

input.o: input.cpp input.hpp
	$(CC) $(CFLAGS) -c input.cpp

clean:
	rm -f *core *.o pacman
