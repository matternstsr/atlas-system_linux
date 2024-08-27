# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g

# Source files
SRC = $(wildcard *.c)

# Object files
OBJ = $(SRC:.c=.o)

# Targets
TARGETS = strace_0 strace_1 strace_2 strace_3

# Default target
all: $(TARGETS)

# Pattern rule for building each target
%: %.o
	$(CC) $(CFLAGS) $^ -o $@

# Specific rules for building object files for each target
strace_0: $(patsubst %.c, %.o, $(wildcard 0-*.c))
strace_1: $(patsubst %.c, %.o, $(wildcard 1-*.c))
strace_2: $(patsubst %.c, %.o, $(wildcard 2-*.c))
strace_3: $(patsubst %.c, %.o, $(wildcard 3-*.c))

# Generic rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGETS) *.o
