# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g

# Add source files for each version if they are different
SRC_0 = strace_0.c
SRC_1 = strace_1.c
SRC_2 = strace_2.c
SRC_3 = strace_3.c

# Default rule to build all targets
all: strace_0 strace_1 strace_2 strace_3

strace_0: $(SRC_0)
	$(CC) $(CFLAGS) -o $@ $^

strace_1: $(SRC_1)
	$(CC) $(CFLAGS) -o $@ $^

strace_2: $(SRC_2)
	$(CC) $(CFLAGS) -o $@ $^

strace_3: $(SRC_3)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f strace_0 strace_1 strace_2 strace_3


# Rule to check for memory leaks using Valgrind
valgrind: $(TARGETS)
	valgrind ./strace_0 /bin/echo hello
	valgrind ./strace_1 /bin/echo hello
	valgrind ./strace_2 /bin/echo hello
	valgrind ./strace_3 /bin/echo hello

.PHONY: all clean valgrind
