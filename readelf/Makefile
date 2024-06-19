CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

SRCS = 0-hreadelf.c elf_common.c endian.c readelfhdrs.c
OBJS = $(SRCS:.c=.o)
EXEC = 0-hreadelf

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
