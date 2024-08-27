# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# Define the executables
TARGETS = strace_0 strace_1 strace_2 strace_3

# Default rule
all: $(TARGETS)

# Rule to build strace_0
strace_0: strace_0.o
	$(CC) $(CFLAGS) -o strace_0 strace_0.o

# Rule to build strace_1
strace_1: strace_1.o
	$(CC) $(CFLAGS) -o strace_1 strace_1.o

# Rule to build strace_2
strace_2: strace_2.o
	$(CC) $(CFLAGS) -o strace_2 strace_2.o

# Rule to build strace_3
strace_3: strace_3.o
	$(CC) $(CFLAGS) -o strace_3 strace_3.o

# Generic rule for building object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build
clean:
	rm -f $(TARGETS) *.o

# Rule to check for memory leaks (using Valgrind for example)
valgrind: $(TARGETS)
	valgrind ./strace_0 /bin/echo hello
	valgrind ./strace_1 /bin/echo hello
	valgrind ./strace_2 /bin/echo hello
	valgrind ./strace_3 /bin/echo hello

.PHONY: all clean valgrind
