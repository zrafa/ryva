
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lgsl -lgslcblas -lm -lGL -lGLU -lglut -lpthread

all: matrix_example

matrix_example: main.c display.c nav_utils.c matrix.c
	g++ -g -c -o magn.o magn.cpp `pkg-config --cflags opencv4`
	gcc -g -c -o main.o main.c
	gcc -g -c -o display.o display.c
	gcc -g -c -o matrix.o matrix.c
	gcc -g -c -o nav_utils.o nav_utils.c
	# $(CC) $(CFLAGS) -o $@ $^ magn.o $(LDFLAGS) $(pkg-config --libs opencv4)
	g++ -g -o $@ main.o display.o matrix.o nav_utils.o magn.o $(LDFLAGS) `pkg-config --libs opencv4`

clean:
	rm -f matrix_example
