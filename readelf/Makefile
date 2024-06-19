CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
# LDFLAGS = -lelf  # Add any additional libraries needed here

# Common source files
COMMON_SRCS = elf_common.c endian.c elf_header.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Additional source files specific to 2-hreadelf
SPECIFIC_SRCS = 2-hreadelf.c 2-hreadelfhelpers.c
SPECIFIC_OBJS = $(SPECIFIC_SRCS:.c=.o)

# Target 2-hreadelf
2-hreadelf: $(COMMON_OBJS) $(SPECIFIC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile common source files into object files
$(COMMON_OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile specific source files into object files
$(SPECIFIC_OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

all: 2-hreadelf

clean:
	rm -f $(COMMON_OBJS) $(SPECIFIC_OBJS) 2-hreadelf
