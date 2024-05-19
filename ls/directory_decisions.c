#include "directory_reader.h"

/**
 * first_alphabetical_string - Returns the string that comes first alphabetically.
 * @strpt1: Pointer to the first string.
 * @strpt2: Pointer to the second string.
 * Return: The string that comes first alphabetically.
 **/
char *first_alphabetical_string(char *strpt1, char *strpt2)
{
    char *result;
    if (*strpt1 == '\0')
        return (*strpt2 ? strpt1 : NULL);
    if (*strpt2 == '\0')
        return (strpt2);
    if (*strpt1 == '.')
        return (first_alphabetical_string(strpt2, strpt1 + 1) == strpt1 + 1 ? strpt1 : strpt2);
    if (*strpt2 == '.')
        return (first_alphabetical_string(strpt1, strpt2 + 1) == strpt2 + 1 ? strpt2 : strpt1);
    if (*strpt1 + 32 * ISUPPER(*strpt1) == *strpt2 + 32 * ISUPPER(*strpt2))
    {
        result = first_alphabetical_string(strpt1 + 1, strpt2 + 1);
        if (result == strpt1 + 1)
            return (strpt1);
        if (result == strpt2 + 1)
            return (strpt2);
        if (*strpt1 == *strpt2)
            return (NULL);
        return (ISLOWER(*strpt1) ? strpt1 : strpt2);
    }
    return ((*strpt1 + 32 * ISUPPER(*strpt1)) < (*strpt2 + 32 * ISUPPER(*strpt2)) ? strpt1 : strpt2);
}

/**
 * should_open_directory - Determines whether a directory should be opened for recursion.
 * @directory_node: File node of the directory.
 * @flags: Flags indicating the options passed onto the program.
 * Return: 1 if opening is approved, 0 if rejected.
 **/
bool should_open_directory(file_node_t *directory_node, ls_config_t *flags)
{
    char *last_char;
    if (S_ISDIR(directory_node->info->st_mode) == false)
        return false;
    if (directory_node->name[0] == '.')
    {
        if (IS_PARENT_DIR(directory_node->name) || IS_CWD(directory_node->name))
            return false;
        if (flags->dot == false && flags->dot_alt == false)
            return false;
    }
    last_char = directory_node->name + str_len(directory_node->name) - 1;
    if (last_char - 2 >= directory_node->name && (IS_CWD(last_char) || IS_PARENT_DIR((last_char - 1))))
        return false;
    return true;
}
