CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu89 -g

SRC0 = hnm.c print_tables.c get_symbols.c swap_all_endian.c print.c get_elf_file_type.c swap_endian_uint64.c swap_endian_uint16.c swap_endian_uint32.c is_elf_file.c crack_open_file.c get.c read.c
OBJ0 = $(SRC0:.c=.o)
NAME0 = hnm

SRC1 = hobjdump.c print_tables.c get_symbols.c swap_all_endian.c print.c get_elf_file_type.c swap_endian_uint64.c swap_endian_uint16.c swap_endian_uint32.c is_elf_file.c crack_open_file.c get.c read.c
OBJ1 = $(SRC1:.c=.o)
NAME1 = hobjdump

.PHONY: all clean oclean fclean re

all: $(NAME0) $(NAME1)

$(NAME0): $(OBJ0)
	$(CC) $(OBJ0) -o $(NAME0)

$(NAME1): $(OBJ1)
	$(CC) $(OBJ1) -o $(NAME1)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *~ $(NAME0) $(NAME1) $(OBJ0) $(OBJ1)

oclean:
	$(RM) $(OBJ0) $(OBJ1)

fclean: clean oclean

re: oclean all
