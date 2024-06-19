CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

SRCS = elf.c 0-hreadelf.c print_helpers.c sparcbigendian32.c
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
