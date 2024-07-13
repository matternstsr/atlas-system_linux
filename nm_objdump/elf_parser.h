#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>

int process_file(const char *filename);

#endif /* ELF_PARSER_H */
