# Define the compiler and compilation flags
COMPILER = gcc
COMPILER_FLAGS = -Wall -Werror -Wextra -pedantic -g

# Collect all source files
SOURCE_FILES = $(wildcard *.c)
# Define object files for each category
OBJECTS_0 = $(patsubst %.c, %.o, $(wildcard 0-*.c))
OBJECTS_1 = $(patsubst %.c, %.o, $(wildcard 1-*.c))
OBJECTS_2 = $(patsubst %.c, %.o, $(wildcard 2-*.c))
OBJECTS_3 = $(patsubst %.c, %.o, $(wildcard 3-*.c))

# Declare phony targets
.PHONY: trace_0 trace_1 trace_2 trace_3 clean

# Rules for building each executable
trace_0: $(OBJECTS_0)
	$(COMPILER) $(COMPILER_FLAGS) $(OBJECTS_0) -o trace_0

trace_1: $(OBJECTS_1)
	$(COMPILER) $(COMPILER_FLAGS) $(OBJECTS_1) -o trace_1

trace_2: $(OBJECTS_2)
	$(COMPILER) $(COMPILER_FLAGS) $(OBJECTS_2) -o trace_2

trace_3: $(OBJECTS_3)
	$(COMPILER) $(COMPILER_FLAGS) $(OBJECTS_3) -o trace_3

# Clean rule to remove generated files
clean:
	rm -f trace_0 trace_1 trace_2 trace_3
	rm -f *.o
