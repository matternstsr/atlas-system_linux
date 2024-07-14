CC = gcc
CFLAGS = -g -Wall -O2
LIBS = -lelf -lm

all: hnm

hnm: hnm.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -static-libgcc -static-libstdc++

clean:
	rm -f hnm
