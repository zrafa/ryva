#
# build sdl2-imgrotate.cpp - A simple SDL2 app that rotates an image on the screen
#
# You need libsdl2-dev package installed
#

# replace the gcc compiler if needed

CC = gcc
CFLAGS = -Wall 
LDFLAGS = -lSDL2 -lSDL2_image

app = bru

all: $(app)

$(app): $(app.c)
	$(CC) $(app).c -o $@ $(LDFLAGS)

clean:
	rm *.o $(app)
