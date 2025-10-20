
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lgsl -lgslcblas -lm -lGL -lGLU -lglut -lpthread

all: matrix_example

matrix_example: main.c display.c nav_utils.c matrix.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f matrix_example
