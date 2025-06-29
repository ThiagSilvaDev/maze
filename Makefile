CC=gcc
CFLAGS=`sdl2-config --cflags` -Wall
LDFLAGS=`sdl2-config --libs`

all: maze

maze: maze.c
	$(CC) $(CFLAGS) -o maze maze.c $(LDFLAGS)

clean:
	rm -f maze