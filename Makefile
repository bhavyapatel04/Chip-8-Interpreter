CC = gcc
SDL_PATH = $(shell brew --prefix sdl2)
CFLAGS = -std=c11 -Wall -Wextra -Werror -I$(SDL_PATH)/include $(shell sdl2-config --cflags)
LDFLAGS = -L$(SDL_PATH)/lib $(shell sdl2-config --libs)

all: chip8

chip8: chip8.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f chip8

.PHONY: all clean
