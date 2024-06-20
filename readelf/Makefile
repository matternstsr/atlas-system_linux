CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# Source files for each target
COMMON_SRCS = elf_common.c endian.c elf_header.c
0-HREADELF_SRCS = 0-hreadelf.c $(COMMON_SRCS)
1-HREADELF_SRCS = 1-hreadelf.c elfprocess.c endianess.c print_sec_hdrs.c \
                  print_elf_flags_key.c check_elf_magic.c convert_flags_to_string.c \
                  res_spec_sec_type_name.c resolve_std_sec_type_name.c \
                  map_spec_sec_type_name.c map_file_into_memory.c

# Object files
0-HREADELF_OBJS = $(0-HREADELF_SRCS:.c=.o)
1-HREADELF_OBJS = $(1-HREADELF_SRCS:.c=.o)

# Targets
0-hreadelf: $(0-HREADELF_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

1-hreadelf: $(1-HREADELF_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilation rules
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies
0-hreadelf.o: 0-hreadelf.c $(COMMON_SRCS)
1-hreadelf.o: 1-hreadelf.c 1-hreadelf.h $(COMMON_SRCS)

# Phony targets
.PHONY: all clean

all: 0-hreadelf 1-hreadelf

clean:
	rm -f $(0-HREADELF_OBJS) $(1-HREADELF_OBJS) 0-hreadelf 1-hreadelf
