Task Breakdown 

    Understanding ELF Format:
        ELF files have sections and symbols that define the structure and executable code/data within the file.
        Symbols can be functions, variables, or other named entities defined within the file.

    Parsing ELF Files:
        Use the ELF data structures defined in /usr/include/elf.h.
        Read the ELF header to understand the structure of the file.
        Traverse sections and symbols to gather necessary information.

    Output Format:
        The output should match the format produced by nm -p, which lists symbols along with their types (e.g., T for text segment symbols, D for data segment symbols, etc.) and addresses.

    Implementation Details:
        Implement the parsing logic in C.
        Handle both 32-bit and 64-bit ELF files.
        Account for both little and big endian formats.

    Makefile:
        Create a Makefile that compiles the hnm executable.
        Define rules to compile the program with gcc 4.8.4 using required flags (-Wall -Werror -Wextra -pedantic).

Example Output

Here’s an example output based on your description:

```bash
0000000000600e20 d __JCR_LIST__
0000000000400630 t deregister_tm_clones
0000000000400670 t register_tm_clones
00000000004006b0 t __do_global_dtors_aux
0000000000601068 b completed.7585
0000000000600e18 t __do_global_dtors_aux_fini_array_entry
00000000004006d0 t frame_dummy
0000000000600e10 t __frame_dummy_init_array_entry
0000000000400b20 r __FRAME_END__
0000000000600e20 d __JCR_END__
0000000000600e18 t __init_array_end
0000000000600e28 d _DYNAMIC
0000000000600e10 t __init_array_start
00000000004009f8 r __GNU_EH_FRAME_HDR
0000000000601000 d _GLOBAL_OFFSET_TABLE_
0000000000400980 T __libc_csu_fini
                 w _ITM_deregisterTMCloneTable
0000000000601058 W data_start
                 U write@@GLIBC_2.2.5
0000000000601068 D _edata
0000000000400984 T _fini
                 U __stack_chk_fail@@GLIBC_2.4
                 U close@@GLIBC_2.2.5
                 U dprintf@@GLIBC_2.2.5
                 U read@@GLIBC_2.2.5
                 U __libc_start_main@@GLIBC_2.2.5
0000000000601058 D __data_start
                 w __gmon_start__
0000000000601060 D __dso_handle
0000000000400990 R _IO_stdin_used
0000000000400910 T __libc_csu_init
0000000000601070 B _end
0000000000400600 T _start
0000000000601068 B __bss_start
00000000004006f6 T main
                 U open@@GLIBC_2.2.5
                 w _Jv_RegisterClasses
                 U exit@@GLIBC_2.2.5
0000000000601068 D __TMC_END__
                 w _ITM_registerTMCloneTable
0000000000400540 T _init

```

Example Implementation Steps

Here’s a simplified outline of how you might structure your C code for hnm:

    Main Program (hnm.c):
        Parse command-line arguments to handle multiple input files.
        Open each file, read its ELF header, and extract sections and symbols.
        Sort and output symbols in the required format.

    ELF Parsing Functions (elf_parser.c, elf_parser.h):
        Implement functions to read ELF headers, sections, and symbols.
        Handle both 32-bit and 64-bit ELF formats.
        Account for endianness differences.

    Makefile:
        Define rules to compile hnm.c and elf_parser.c.
        Link them together to create the hnm executable.

Conclusion

Implementing a tool like hnm involves a deep dive into ELF file structures and careful parsing and formatting of symbol information. The provided structure and steps should guide you through creating a functional nm-like utility that meets all specified requirements.


Directory Structure:

```bash
└── nm_objdump
    ├── Makefile
    ├── hnm.c
    ├── elf_parser.c
    ├── elf_parser.h
    └── README.md

```

```bash
Makefile
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

```

hnm.c

This file will handle the main program logic, including command-line argument parsing and coordinating with the ELF parser.

elf_parser.h

Header file declaring functions for ELF file parsing.

elf_parser.c

Implementation of ELF file parsing functions.

