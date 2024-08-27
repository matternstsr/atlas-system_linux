# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g

# Define the source files and targets
SRC = $(wildcard *.c)
OBJ_0 = $(patsubst %.c, %.o, $(wildcard 0-*.c))
OBJ_1 = $(patsubst %.c, %.o, $(wildcard 1-*.c))
OBJ_2 = $(patsubst %.c, %.o, $(wildcard 2-*.c))
OBJ_3 = $(patsubst %.c, %.o, $(wildcard 3-*.c))

TARGETS = strace_0 strace_1 strace_2 strace_3

# Default rule to build all targets
all: $(TARGETS)

# Rules to build each target
strace_0: $(OBJ_0)
	$(CC) $(CFLAGS) $(OBJ_0) -o strace_0

strace_1: $(OBJ_1)
	$(CC) $(CFLAGS) $(OBJ_1) -o strace_1

strace_2: $(OBJ_2)
	$(CC) $(CFLAGS) $(OBJ_2) -o strace_2

strace_3: $(OBJ_3)
	$(CC) $(CFLAGS) $(OBJ_3) -o strace_3

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build
clean:
	rm -f $(TARGETS) *.o

# Rule to check for memory leaks using Valgrind
valgrind: $(TARGETS)
	valgrind ./strace_0 /bin/echo hello
	valgrind ./strace_1 /bin/echo hello
	valgrind ./strace_2 /bin/echo hello
	valgrind ./strace_3 /bin/echo hello

.PHONY: all clean valgrind
