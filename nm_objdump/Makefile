CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
LDFLAGS =

SRC = hnm.c elf_parser.c
OBJ = $(SRC:.c=.o)
TARGET = hnm

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
