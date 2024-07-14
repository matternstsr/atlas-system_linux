CC = gcc
CFLAGS = -g -Wall
LIBS = -lelf

all: hnm

hnm: hnm.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f hnm
