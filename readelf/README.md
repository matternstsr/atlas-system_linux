# Project: readelf

This repository contains a collection of C programs designed to parse and display information from ELF files, adhering to specific requirements and standards.

## Curriculum
- [C#22] 2023FA-TUL - LAB - T3
- Average: 89.77%

## Project Badge
50%

## C - ELF: readelf Master
By: Alexandre Gautier, Software Engineer at Holberton School
Weight: 1

## Migrated to checker v2

Your score will be updated as you progress.

## Resources
### Read or watch:
- [ELF Wikipedia](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)
- [ELF: Understanding and Analysis](https://man7.org/linux/man-pages/man5/elf.5.html)

### man or help:
- elf (5)
- readelf (1)

## Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

### General
- What is the ELF format
- What kind of files are in the ELF format
- What are the different parts that constitute an ELF file
- What information is present in the file header
- What information is present in the sections header table
- What information is present in the program header table
- How to parse an ELF file using C structures

## Requirements
### General
- Allowed editors: vi, vim, emacs
- All your files will be compiled on Ubuntu 14.04 LTS
- Your C programs and functions will be compiled with gcc 4.8.4 using the flags -Wall -Werror -Wextra and -pedantic
- All your files should end with a new line
- A README.md file, at the root of the folder of the project, is mandatory
- Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
- You are not allowed to have more than 5 functions per file
- All your header files should be include guarded

### Betty Compliance
All the C source files in your directory and subdirectories must be Betty-compliant

### Allowed Functions and System Calls
- Unless specified otherwise, you are allowed to use the C standard library
- You’re not allowed to use system(3)
- You’re not allowed to use exec*(2 and 3)

### Compilation
- You have to provide a Makefile in order to compile each task
- Make sure each task compiles on Ubuntu 14.04 LTS, with gcc 4.8.4
- You must compile each task using the flags -Wall -Werror -Wextra and -pedantic

### Tests
- Your program must be able to handle both 32-bit and 64-bit ELF files
- Your program must be able to handle both little and big endian ELF files
- Your program must be able to handle all types of ELF files

## More Info
### Extra Reading
- Check out /usr/include/elf.h

## Repo:
- GitHub repository: atlas-system_linux
- Directory: readelf
- File: Makefile

---

## Tasks

### 0. ELF file header
- **Score:** 50.00% (Checks completed: 100.00%)
- Write a program that displays the information contained in the ELF file header of an ELF file.

#### Usage:
```shell
0-hreadelf elf_filename
```

## Example Output:

```shell
$ make 0-hreadelf
$ ./0-hreadelf ubuntu64
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x400600
  Start of program headers:          64 (bytes into file)
  Start of section headers:          6936 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         31
  Section header string table index: 28
  ```

1. ELF sections' headers
Score: 50.00% (Checks completed: 100.00%)
Write a program that displays the information contained in the ELF sections' headers of an ELF file.

```shell
1-hreadelf elf_filename
```

```shell
$ make 1-hreadelf
$ ./1-hreadelf ubuntu64
```

```shell
There are 31 section headers, starting at offset 0x1b18:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .interp           PROGBITS        0000000000400238 000238 00001c 00   A  0   0  1
  [ 2] .note.ABI-tag     NOTE            0000000000400254 000254 000020 00   A  0   0  4
  [ 3] .note.gnu.build-id NOTE            0000000000400274 000274 000024 00   A  0   0  4
  [ 4] .gnu.hash         GNU_HASH        0000000000400298 000298 00001c 00   A  5   0  8
  [ 5] .dynsym           DYNSYM          00000000004002b8 0002b8 0000f0 18   A  6   1  8
  [ 6] .dynstr           STRTAB          00000000004003a8 0003a8 000076 00   A  0   0  1
  [ 7] .gnu.version      VERSYM          000000000040041e 00041e 000014 02   A  5   0  2
  [ 8] .gnu.version_r    VERNEED         0000000000400438 000438 000030 00   A  6   1  8
  [ 9] .rela.dyn         RELA            0000000000400468 000468 000018 18   A  5   0  8
  [10] .rela.plt         RELA            0000000000400480 000480 0000c0 18  AI  5  24  8
  [11] .init             PROGBITS        0000000000400540 000540 00001a 00  AX  0   0  4
  [12] .plt              PROGBITS        0000000000400560 000560 000090 10  AX  0   0 16
  [13] .plt.got          PROGBITS        00000000004005f0 0005f0 000008 00  AX  0   0  8
  [14] .text             PROGBITS        0000000000400600 000600 000382 00  AX  0   0 16
  [15] .fini             PROGBITS        0000000000400984 000984 000009 00  AX  0   0  4
  [16] .rodata           PROGBITS        0000000000400990 000990 000067 00   A  0   0  8
  [17] .eh_frame_hdr     PROGBITS        00000000004009f8 0009f8 000034 00   A  0   0  4
  [18] .eh_frame         PROGBITS        0000000000400a30 000a30 0000f4 00   A  0   0  8
  [19] .init_array       INIT_ARRAY      0000000000600e10 000e10 000008 00  WA  0   0  8
  [20] .fini_array       FINI_ARRAY      0000000000600e18 000e18 000008 00  WA  0   0  8
  [21] .jcr              PROGBITS        0000000000600e20 000e20 000008 00  WA  0   0  8
  [22] .dynamic          DYNAMIC         0000000000600e28 000e28 0001d0 10  WA  6   0  8
  [23] .got              PROGBITS        0000000000600ff8 000ff8 000008 08  WA  0   0  8
  [24] .got.plt          PROGBITS        0000000000601000 001000 000038 08  WA  0   0  8
  [25] .data             PROGBITS        0000000000601038 001038 000030 00  WA  0   0  8
  [26] .bss              NOBITS          0000000000601068 001068 000010 00  WA  0   0  1
  [27] .comment          PROGBITS        0000000000000000 001068 000033 01  MS  0   0  1
  [28] .shstrtab         STRTAB          0000000000000000 00109b 00011d 00      0   0  1
  [29] .symtab           SYMTAB          0000000000000000 0011b8 000780 18     30  41  8
  [30] .strtab           STRTAB          0000000000000000 001938 00034f 00      0   0  1
```
### 2. ELF program headers

- **Score:** 50.00% (Checks completed: 100.00%)
- Write a program that displays the information contained in the ELF program headers of an ELF file.

#### Usage:

```shell
2-hreadelf elf_filename
```

##### Example Output:

```shell
$ make 2-hreadelf
$ ./2-hreadelf ubuntu64
```

```shell
Elf file type is EXEC (Executable file)
Entry point 0x400600
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000001f8 0x00000000000001f8  R E    8
  INTERP         0x0000000000000238 0x0000000000400238 0x0000000000400238
                 0x000000000000001c 0x000000000000001c  R      1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x0000000000000f50 0x0000000000000f50  R E    200000
  LOAD           0x0000000000000db8 0x0000000000600db8 0x0000000000600db8
                 0x0000000000000230 0x0000000000000240  RW     200000
  DYNAMIC        0x0000000000000dc8 0x0000000000600dc8 0x0000000000600dc8
                 0x00000000000001d0 0x00000000000001d0  RW     8
  NOTE           0x0000000000000254 0x0000000000400254 0x0000000000400254
                 0x0000000000000020 0x0000000000000020  R      4
  GNU_EH_FRAME   0x00000000000009f8 0x00000000004009f8 0x00000000004009f8
                 0x0000000000000034 0x0000000000000034  R      4
  GNU_STACK      0x0000000000000000 0x000000000000
```

### Files

```shell
| File Name       | Description                                  |
|-----------------|----------------------------------------------|
| `README.md`     | This file. Contains project documentation.   |
| `2-hreadelf.c`  | C source code for parsing ELF program headers. |
| `Makefile`      | Makefile for compiling the `2-hreadelf` program. |
| `elf_reader.h`  | Header file containing function prototypes and constants. |
| `elf_reader.c`  | Implementation file for ELF file parsing functions. |
| `ubuntu64`      | Example ELF executable file used for testing. |
| `.gitignore`    | Git ignore file.                              |
```

#### Directory Structure

├── README.md
├── 2-hreadelf.c
├── Makefile
├── elf_reader.h
├── elf_reader.c
├── ubuntu64
└── .gitignore

### Additional Resources

- **ELF Wikipedia page**
  [ELF Wikipedia](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

- **readelf command documentation (man readelf)**
  You can access the `readelf` command documentation using the `man` pages in your terminal:

```shell
man readelf
```


- **/usr/include/elf.h for C programming reference**
This file provides reference information for programming with ELF in C. It can typically be found in the `/usr/include` directory on Unix-like systems.

- **Author**
Matthew Ernst - [atlas-system_linux/matternstsr](https://github.com/matternstsr)
- **Repository**
GitHub: [atlas-system_linux/readelf](https://github.com/matternstsr/atlas-system_linux/tree/main/readelf)





