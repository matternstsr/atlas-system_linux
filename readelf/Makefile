CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# Source files for each target
COMMON_SRCS = 0elf_common.c 1endian.c 0elf_header.c
0-HREADELF_SRCS = 0-hreadelf.c $(COMMON_SRCS)
1-HREADELF_SRCS = 1-hreadelf.c 1elfprocess.c 1endianess.c 1print_sec_hdrs.c \
                  1print_elf_flags_key.c 1check_elf_magic.c 1convert_flags_to_string.c \
                  1res_spec_sec_type_name.c 1resolve_std_sec_type_name.c \
                  1map_spec_sec_type_name.c 1map_file_into_memory.c
2-HREADELF_SRCS = 2-hreadelf.c 2-open_and_handle_file.c 2-check_elf_header.c \
2-switch_all_endian.c 2-display_all_elf_program_headers.c 2-switch_2_byte.c \
2-readit.c 2-get_it_all.c 2-mapping.c 2-print_programs.c $(COMMON_SRCS)

# Object files
0-HREADELF_OBJS = $(0-HREADELF_SRCS:.c=.o)
1-HREADELF_OBJS = $(1-HREADELF_SRCS:.c=.o)
2-HREADELF_OBJS = $(2-HREADELF_SRCS:.c=.o)

# Targets
0-hreadelf: $(0-HREADELF_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

1-hreadelf: $(1-HREADELF_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

2-hreadelf: $(2-HREADELF_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilation rules
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies
0-hreadelf.o: 0-hreadelf.c $(COMMON_SRCS)
1-hreadelf.o: 1-hreadelf.c 1-hreadelf.h $(COMMON_SRCS)
2-hreadelf.o: 2-hreadelf.c 2-hreadelf.h $(COMMON_SRCS)

# Phony targets
.PHONY: all clean

all: 0-hreadelf 1-hreadelf 2-hreadelf

clean:
	rm -f $(0-HREADELF_OBJS) $(1-HREADELF_OBJS) $(2-HREADELF_OBJS) 0-hreadelf 1-hreadelf 2-hreadelf
