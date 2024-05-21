/* atlas-system_linux/ls/directory_decisions.c */

#include "directory_reader.h"

/**
 * first_alphabetical_string - Returns the string that comes first.
 * @strpt1: Pointer to the first string.
 * @strpt2: Pointer to the second string.
 * Return: The string that comes first alphabetically.
 **/
char *first_alphabetical_string(char *strpt1, char *strpt2)
{
    char *result;
    if (*strpt1 == '\0')
        return (*strpt2 ? strpt1 : NULL); /* If strpt1 is empty, return strpt1 if strpt2 is not empty, otherwise return NULL */
    if (*strpt2 == '\0')
        return (strpt2); /* If strpt2 is empty, return strpt2 */
    if (*strpt1 == '.')
        return (first_alphabetical_string(strpt2, strpt1 + 1) == strpt1 + 1 ? strpt1 : strpt2); /* If strpt1 starts with '.', compare strpt1+1 with strpt2 */
    if (*strpt2 == '.')
        return (first_alphabetical_string(strpt1, strpt2 + 1) == strpt2 + 1 ? strpt2 : strpt1); /* If strpt2 starts with '.', compare strpt2+1 with strpt1 */
    if (*strpt1 + 32 * ISUPPER(*strpt1) == *strpt2 + 32 * ISUPPER(*strpt2))
    {
        result = first_alphabetical_string(strpt1 + 1, strpt2 + 1); /* If strpt1 and strpt2 have the same character (case insensitive), compare the next characters */
        if (result == strpt1 + 1)
            return (strpt1); /* If strpt1 comes first alphabetically, return strpt1 */
        if (result == strpt2 + 1)
            return (strpt2); /* If strpt2 comes first alphabetically, return strpt2 */
        if (*strpt1 == *strpt2)
            return (NULL); /* If strpt1 and strpt2 are equal, return NULL */
        return (ISLOWER(*strpt1) ? strpt1 : strpt2); /* If strpt1 and strpt2 are not equal, return the one with lower case */
    }
    return ((*strpt1 + 32 * ISUPPER(*strpt1)) < (*strpt2 + 32 * ISUPPER(*strpt2)) ? strpt1 : strpt2); /* Compare strpt1 and strpt2, considering case */
}


/**
 * should_open_directory - Determines whether a directory should be opened for recursion.
 * @directory_node: File node of the directory.
 * @flags: Flags indicating the options passed onto the program.
 * Return: 1 if opening is approved, 0 if rejected.
 **/
bool should_open_directory(file_node_t *directory_node, ls_flag_t *flags)
{
    char *last_char;
    
    /* Check if directory */
    if (S_ISDIR(directory_node->info->st_mode) == false)
        return false;
    
    /* Check for directory starting with '.' */
    if (directory_node->name[0] == '.')
    {
        /* Exclude . and .. */
        if (IS_PARENT_DIR(directory_node->name) || IS_CWD(directory_node->name))
            return false;
        /* Check if dot options are disabled */
        if (flags->dot == false && flags->dot_alt == false)
            return false;
    }
    
    /* Check for special directories . and .. */
    last_char = directory_node->name + str_len(directory_node->name) - 1;
    if (last_char - 2 >= directory_node->name && (IS_CWD(last_char) || IS_PARENT_DIR((last_char - 1))))
        return false;
    
    /* All conditions passed, approve opening */
    return true;
}

