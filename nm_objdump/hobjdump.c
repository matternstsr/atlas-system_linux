#include "hnm.h"

/**
 * main - entry point to hobjdump
 * @argc: argument count from command line
 * @argv: argument array from command line
 * Return: 1 if any file processing fails, 0 otherwise
 */
int main(int argc, char **argv)
{
    char *default_args[] = {"a.out"};
    char **args = argc > 1 ? argv + 1 : default_args;
    objdump_state state;
    int i, retval, ac = argc > 1 ? argc - 1 : 1;

    /* hobjdump processes one or more ELF files */
    /* Each file failure results in overall failure (exit code 1) */
    for (i = 0, retval = 0; i < ac; i++)
    {
        initState(&state);
        state.exec_name = argv[0];
        state.f_name = args[i];

        if (openELF(&state) == 0)
        {
            if (readElfFileHeader(&state) == 0)
            {
                if (!(getSecHeaders(&state) == 0 &&
                      getSecHeadStrTab(&state) == 0 &&
                      printFileInfo(&state) == 0 &&
                      printSections(&state) == 0))
                    retval = 1;
            }
            else
            {
                errorMsg("%s: File format not recognized\n",
                         NULL, &state);
                retval = 1;
            }
        }
        else
            retval = 1;
        closeState(&state);
    }
    return (retval);
}
