/* atlas-system_linux/ls/main.c */

#include "directory_reader.h"

/**
 * main - Custom implementation of the ls command.
 * @argc: Argument count.
 * @argv: Argument array.
 * Return: Exit status: 0 on success, 2 on failure.
 **/
int main(int argc, char **argv)
{
    int index, status = 0, check = 0, dir_count = 0;
    dir_node_t *dirs_list = NULL;
    file_node_t *file_list = NULL;
    DIR *dir_stream;
    ls_flag_t flags = {&print_short_list, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (index = 1; index < argc; index++, status = check ? check : status)
        if (argv[index][0] == '-' && argv[index][1] != '\0')
        {
            if (flag_interpreter(argv[index], &flags))
            {
                free_it_all(dirs_list, file_list);
                return (2);
            }
        }
        else
        {
            dir_stream = opendir(argv[index]);
            if (dir_stream == NULL && (errno == ENOTDIR || errno == ENOENT))
                check = add_file(argv[index], "", &file_list);
            else
                check = add_dir(argv[index], dir_stream, &dirs_list), dir_count++;
        }

    if (file_list || dir_count > 1 || (file_list == NULL && status != 0))
        flags.print_dir_name = true;
    if (dir_count == 0 && file_list == NULL && status == 0)
        add_dir(".", opendir("."), &dirs_list);
    flags.printer(file_list, &flags);
    if (dir_count && file_list)
        putchar('\n');
    check = print_dirs(&dirs_list, &flags, flags.printer);
    status = check ? check : status;
    free_it_all(dirs_list, file_list);
    return (status ? 2 : 0);
}