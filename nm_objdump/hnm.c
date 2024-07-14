#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include "elf_parser.h"

void print_symbols(Elf *elf)
{
    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    Elf_Data *data;
    Elf64_Sym *symtab;
    int count, i;

    while ((scn = elf_nextscn(elf, scn)) != NULL)
    {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM)
        {
            data = elf_getdata(scn, NULL);
            symtab = (Elf64_Sym *)data->d_buf;
            count = shdr.sh_size / shdr.sh_entsize;

            for (i = 0; i < count; i++)
            {
                if (ELF64_ST_TYPE(symtab[i].st_info) != STT_FILE)
                {
                    char type;
                    switch (ELF64_ST_TYPE(symtab[i].st_info))
                    {
                        case STT_NOTYPE:
                            type = ' ';
                            break;
                        case STT_OBJECT:
                            type = 'd';
                            break;
                        case STT_FUNC:
                            type = 'T';
                            break;
                        case STT_SECTION:
                            type = ' ';
                            break;
                        case STT_FILE:
                            type = ' ';
                            break;
                        case STT_COMMON:
                            type = ' ';
                            break;
                        case STT_TLS:
                            type = 'T';
                            break;
                        default:
                            type = '?';
                            break;
                    }

                    printf("%016lx %c %s\n", (unsigned long)symtab[i].st_value,
                           type, elf_strptr(elf, shdr.sh_link, symtab[i].st_name));
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int fd, i;
    Elf *elf;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [objfile...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    elf_version(EV_CURRENT);

    for (i = 1; i < argc; i++)
    {
        if ((fd = open(argv[i], O_RDONLY, 0)) < 0)
        {
            perror("open");
            continue;
        }

        if ((elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
        {
            perror("elf_begin");
            close(fd);
            continue;
        }

        printf("\n%s:\n", argv[i]);
        print_symbols(elf);

        elf_end(elf);
        close(fd);
    }

    return 0;
}
