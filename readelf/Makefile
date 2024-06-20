CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# Common source files
COMMON_SRCS = elf_common.c endian.c elf_header.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Target 0-hreadelf
0-hreadelf: 0-hreadelf.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

0-hreadelf.o: 0-hreadelf.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target 1-hreadelf
1-hreadelf: 1-hreadelf.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

1-hreadelf.o: 1-hreadelf.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target 1-hreadelf
2-hreadelf: 2-hreadelf.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

2-hreadelf.o: 2-hreadelf.c
	$(CC) $(CFLAGS) -c $< -o $@

# Common object files compilation
$(COMMON_OBJS): $(COMMON_SRCS)
	$(CC) $(CFLAGS) -c $^

.PHONY: all clean

all: 0-hreadelf 1-hreadelf

clean:
	rm -f $(COMMON_OBJS) 0-hreadelf.o 1-hreadelf.o 0-hreadelf 1-hreadelf
