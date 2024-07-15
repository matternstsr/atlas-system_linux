CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu11 -g

SRC0 = hnm.c elf_parser.c
OBJ0 = $(SRC0:.c=.o)
NAME0 = hnm

SRC1 = hobjdump.c elf_parser.c
OBJ1 = $(SRC1:.c=.o)
NAME1 = hobjdump

.PHONY: all clean oclean fclean re

all: $(NAME0) $(NAME1)

$(NAME0): $(OBJ0)
	$(CC) $(OBJ0) -o $(NAME0)

$(NAME1): $(OBJ1)
	$(CC) $(OBJ1) -o $(NAME1)

clean:
	$(RM) *~ $(OBJ0) $(OBJ1) $(NAME0) $(NAME1)

oclean:
	$(RM) $(OBJ0) $(OBJ1)

fclean: clean oclean

re: oclean all

hnm: elf_parser.h $(OBJ0)
	$(CC) $(OBJ0) -o $(NAME0)

hobjdump: elf_parser.h $(OBJ1)
	$(CC) $(OBJ1) -o $(NAME1)
