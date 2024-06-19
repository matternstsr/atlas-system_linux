#include "myelf.h"
#include <stdio.h>

void print_osabi(unsigned char osabi) {
    const char *osabi_str;

    switch (osabi) {
        case ELFOSABI_SYSV:
            osabi_str = "UNIX - System V";
            break;
        case ELFOSABI_HPUX:
            osabi_str = "HP-UX";
            break;
        case ELFOSABI_NETBSD:
            osabi_str = "NetBSD";
            break;
        case ELFOSABI_LINUX:
            osabi_str = "Linux";
            break;
        case ELFOSABI_SOLARIS:
            osabi_str = "Solaris";
            break;
        case ELFOSABI_IRIX:
            osabi_str = "IRIX";
            break;
        case ELFOSABI_FREEBSD:
            osabi_str = "FreeBSD";
            break;
        case ELFOSABI_TRU64:
            osabi_str = "TRU64 UNIX";
            break;
        case ELFOSABI_ARM_AEABI:
            osabi_str = "ARM EABI";
            break;
        case ELFOSABI_STANDALONE:
            osabi_str = "Standalone (embedded) application";
            break;
        default:
            osabi_str = "Unknown";
            break;
    }

    printf("  OS/ABI:                            %s\n", osabi_str);
}

void print_file_type(uint16_t e_type) {
    const char *file_type;

    switch (e_type) {
        case ET_NONE:
            file_type = "None";
            break;
        case ET_REL:
            file_type = "REL (Relocatable file)";
            break;
        case ET_EXEC:
            file_type = "EXEC (Executable file)";
            break;
        case ET_DYN:
            file_type = "DYN (Shared object file)";
            break;
        case ET_CORE:
            file_type = "CORE (Core file)";
            break;
        default:
            file_type = "Unknown";
            break;
    }

    printf("  Type:                              %s\n", file_type);
}

void print_machine_type(uint16_t e_machine) {
    const char *machine_type;

    switch (e_machine) {
        case EM_SPARC:
            machine_type = "Sparc";
            break;
        case EM_386:
            machine_type = "Intel 80386";
            break;
        case EM_ARM:
            machine_type = "ARM";
            break;
        case EM_X86_64:
            machine_type = "AMD x86-64";
            break;
        default:
            machine_type = "Unknown";
            break;
    }

    printf("  Machine:                           %s\n", machine_type);
}
