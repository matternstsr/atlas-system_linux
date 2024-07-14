CC = gcc
CFLAGS = -Wall -g
LDFLAGS =
LDLIBS = -lelf   # Link with libelf

SRCS = hnm.c elf_parser.c
OBJS = $(SRCS:.c=.o)
TARGET = hnm

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
