#ifndef FILE2_H
#define FILE2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Function prototypes */
void solaris32_func(const char *filename);
void sortix32_func(const char *filename);
void sparcbigendian32_func(const char *filename);
void jpeg_mod_func(const char *filename);
void case_python_obj_func(const char *filename);
void vgpreload_memcheck_x86_linux_so_func(const char *filename);

const char *section_type_to_string_func(unsigned int type);
const char *flags_to_string_func(unsigned long flags);
void readelf_sections_func(FILE *fp);

#endif /* FILE2_H */
