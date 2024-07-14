CC = gcc
CFLAGS = -g -Wall -O2
LIBS = -lelf -lm -static-libgcc -static-libstdc++

all: hnm

hnm: hnm.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f hnm
