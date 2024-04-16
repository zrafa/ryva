#
#
# You need libsdl2-dev package installed
#

# replace the gcc compiler if needed

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL2 -lSDL2_image -lm -g

app = mag_sdl

all: $(app)

$(app): $(app.c)
	$(CC) $(app).c -o $@ $(LDFLAGS)

clean:
	rm *.o $(app)

run: $(app)
	stty -F /dev/ttyUSB0 speed 115200
	cat /dev/ttyUSB0 | ./$(app)
	
