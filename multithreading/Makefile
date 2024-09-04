# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -pedantic -g3 -pthread
LDFLAGS = -lm

# Executables
TARGET_0 = 0-thread_entry
TARGET_1 = 1-tprintf
TARGET_10 = 10-blur_portion
TARGET_11 = 11-blur_image
TARGET_22 = 22-prime_factors

# Source files
SRCS_0 = 0-thread_entry.c 0-main.c
SRCS_1 = 1-tprintf.c 1-main.c
SRCS_10 = 10-blur_portion.c list.c 10-main.c
SRCS_11 = 11-blur_image.c list.c 11-main.c
SRCS_22 = 22-prime_factors.c list.c 22-main.c

# Object files
OBJS_0 = $(SRCS_0:.c=.o)
OBJS_1 = $(SRCS_1:.c=.o)
OBJS_10 = $(SRCS_10:.c=.o)
OBJS_11 = $(SRCS_11:.c=.o)
OBJS_22 = $(SRCS_22:.c=.o)

# Default target
all: $(TARGET_0) $(TARGET_1) $(TARGET_10) $(TARGET_11) $(TARGET_22)

# Build target for 0-thread_entry
$(TARGET_0): $(OBJS_0)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build target for 1-tprintf
$(TARGET_1): $(OBJS_1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build target for 10-blur_portion
$(TARGET_10): $(OBJS_10)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build target for 11-blur_image
$(TARGET_11): $(OBJS_11)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build target for 22-prime_factors
$(TARGET_22): $(OBJS_22)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS_0) $(OBJS_1) $(OBJS_10) $(OBJS_11) $(OBJS_22)
	rm -f $(TARGET_0) $(TARGET_1) $(TARGET_10) $(TARGET_11) $(TARGET_22)

# Phony targets
.PHONY: all clean
