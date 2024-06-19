/* main.c */

#include "myelf.h"

/* Main function to execute the program */
int main(int argc, char *argv[]) {
    if (argc != 2) { /* Check for correct number of arguments */
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    sparcbigendian32(argv[1]); /* Call function to print ELF header */

    return EXIT_SUCCESS;
}
