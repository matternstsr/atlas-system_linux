# Makefile

CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic
LDFLAGS=

SRCS = 0-hreadelf.c elf.c
OBJS = $(SRCS:.c=.o)
TARGET = 0-hreadelf

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
