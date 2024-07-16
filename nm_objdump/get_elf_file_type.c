/**
 * get_elf_file_type - Retrieves a string representation of the ELF file type.
 * @elf_header: Pointer to the ELF header structure (elf_t).
 * Return: A constant string describing the ELF file type.
 * This function determines and returns a human-readable string representation
 * of the ELF file type based on the e_type field of the provided elf_header.
 * It uses a static buffer to store and return the string for efficiency, and
 * handles various ELF file types such as NONE, REL, EXEC, DYN, and CORE. If the
 * file type is unknown, it formats a custom string into the buffer to indicate
 * the unknown type with its hexadecimal value.
 */
const char *get_elf_file_type(elf_t *elf_header)
{
    static char buf[32]; /* Static buffer to hold the result string */

    switch (EGET(e_type)) /* Switch based on the e_type field of the elf_header */
    {
        case ET_NONE:
            return ("NONE (None)"); /* Return a string literal indicating "NONE (None)" */
        case ET_REL:
            return ("REL (Relocatable file)"); /* Return a string literal indicating "REL (Relocatable file)" */
        case ET_EXEC:
            return ("EXEC (Executable file)"); /* Return a string literal indicating "EXEC (Executable file)" */
        case ET_DYN:
            return ("DYN (Shared object file)"); /* Return a string literal indicating "DYN (Shared object file)" */
        case ET_CORE:
            return ("CORE (Core file)"); /* Return a string literal indicating "CORE (Core file)" */
        default:
            sprintf(buf, "<unknown>: %x", EGET(e_type)); /* Format a custom message into buf for unknown types */
    }
    return (buf); /* Return the buffer, which now contains the formatted string */
}
