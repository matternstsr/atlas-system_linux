#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <byteswap.h>
#include <sys/mman.h>
#include <stddef.h>
#include <gelf.h>
#include <libelf.h>

int process_file(const char *filename);

#endif /* ELF_PARSER_H */