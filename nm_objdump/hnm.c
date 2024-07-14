#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>

void print_symbols(Elf *elf)
{
    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    Elf_Data *data;
    Elf32_Sym *symtab;
    int count, i;

    while ((scn = elf_nextscn(elf, scn)) != NULL)
    {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM)
        {
            data = elf_getdata(scn, NULL);
            symtab = (Elf32_Sym *)data->d_buf;
            count = shdr.sh_size / shdr.sh_entsize;

            for (i = 0; i < count; i++)
            {
                if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FILE)
                {
                    char bind = ELF32_ST_BIND(symtab[i].st_info);
                    char type;

                    if (bind == STB_LOCAL)
                        type = 't';
                    else if (bind == STB_GLOBAL)
                        type = 'T';
                    else if (bind == STB_WEAK)
                        type = 'W';
                    else if (bind == STB_GNU_UNIQUE)
                        type = 'u';
                    else
                        type = '?';

                    printf("%08x %c %s\n", (unsigned int)symtab[i].st_value,
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
