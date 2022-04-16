CC=gcc
#CFLAGS+=-g
CFLAGS+=-Wall -O2
# LDFLAGS+=-ljpeg
LDFLAGS+=jpeg-9c/.libs/libjpeg.a
LDFLAGS+=-L./jpeg-9c
PROG=cliente_v4l2
SRCS=utils
INCS=-I. -I$(SRCS) -I./jpeg-9c

OBJS =  $(PROG).o \
        $(SRCS)/YUYVtoJPEG.o \
        $(SRCS)/socket.o \
        $(SRCS)/save_file.o \
	tiempos.o

.PHONY: all clean
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS)  $(INCS) $<  -o $@

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG)

