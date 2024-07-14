CC = gcc
CFLAGS = -Wall -g  # -Wall -Werror -Wextra -pedantic -std=gnu90 -g
LIBS = -lelf  # Link with libelf

SRC0 = hnm.c elf_parser.c
OBJ0 = $(SRC0:.c=.o)
NAME0 = hnm

SRC1 = hobjdump.c elf_parser.c
OBJ1 = $(SRC1:.c=.o)
NAME1 = hobjdump

.PHONY: all clean oclean fclean re

all: $(NAME0) $(NAME1)

$(NAME0): $(OBJ0)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(NAME1): $(OBJ1)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	$(RM) *~ $(OBJ0) $(OBJ1) $(NAME0) $(NAME1)

oclean:
	$(RM) $(OBJ0) $(OBJ1)

fclean: clean oclean

re: fclean all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
