/* atlas-system_linux/ls/directory_helpers.c */

#include "directory_reader.h"

/**
 * string_dup - Returns a pointer to a duplicate of the source string.
 * @source_string: The string to be copied to newly allocated memory space.
 * Return: A char pointer to the copied string, or NULL if memory allocation fails.
 */
char *string_dup(char *source_string)
{
    int index, length;
    char *copied_string;

    if (source_string == NULL) /* Check if source string is NULL */ 
        return NULL;
    for (length = 0; source_string[length]; length++) /* Get string length */
        ;
    length++;
    copied_string = (char *)malloc(length); /* Allocate memory */
    if (copied_string == NULL) /* Check for NULL after memory allocation */		
        return NULL;
    for (index = 0; index < length; index++) /* Copy string */
        copied_string[index] = source_string[index];
    return copied_string; /* Return pointer */
}

/**
 * char_search - Returns a pointer to the first instance of a character 'c' in the string 'str'.
 * @str: The string to search.
 * @c: The character to find in the string.
 * Return: A pointer to the first instance of 'c'. If 'c' is not found in 'str', NULL is returned.
 **/
char *char_search(char *str, char c)
{
    /* Check if the string is NULL */
    if (!str)
        return NULL;
    
    /* Iterate through the string */
    while (*str)
        /* If the current character is not equal to 'c', move to the next character */
        if (*str != c)
            str++;
        /* If the current character is equal to 'c', return a pointer to it */
        else
            return str;
    
    /* If 'c' is not found in 'str', return NULL */
    return NULL;
}



/**
 * string_copy - Custom implementation of strcpy.
 * @dest: The destination buffer.
 * @src: The source string.
 * Return: A pointer to dest.
 **/
char *string_copy(char *dest, char *src)
{
    int i;

    if (!dest || !src) /* Check if either dest or src is NULL */
        return (NULL);
    for (i = 0; src[i]; i++) /* Iterate through source string */
    {
        dest[i] = src[i]; /* Copy characters from source to destination */
    }
    dest[i] = '\0'; /* Add null terminator to destination */
    return (dest); /* Return pointer to destination */
}


/**
* str_len - Returns the length of a string.
* @s: The string to be evaluated.
* Return: An integer representing the length of the string.
**/
int str_len(char *s)
{
    int strlen = 0; /* Initialize the variable to store the length of the string. */

    while (s[strlen]) /* Loop through the string until the null terminator is reached. */
        strlen++; /* Increment the length for each character encountered. */
    return (strlen); /* Return the length of the string. */
}

/**
 * flag_interpreter - Sets configurations for the ls command based on given flags.
 * @arg: Argument.
 * @flags: Flags structure.
 * Return: 0 on success, 2 if an invalid option is encountered.
 **/
int flag_interpreter(char *arg, ls_flag_t *flags)
{
    int i;
		
    for (i = 1; arg[i] != '\0'; i++)
        if (arg[i] == '1') /* Set flag for one per line */
            flags->one_per_line = true;
        else if (arg[i] == 'a') /* Set flag for dot */
            flags->dot = true;
        else if (arg[i] == 'A') /* Set flag for dot_alt */
            flags->dot_alt = true;
        else if (arg[i] == 'l') /* Set printer function for long list */
            flags->printer = &print_long_list;
        else if (arg[i] == 'r') /* Set flag for reversed */
            flags->reversed = true;
        else if (arg[i] == 'R') /* Set flag for recursive */
            flags->recursive = true;
        else if (arg[i] == 't') /* Set flag for sort by time */
            flags->sort_by_time = true;
        else if (arg[i] == 'S') /* Set flag for sort by size */
            flags->sort_by_size = true;
        else /* Handle invalid option */
        {
            fprintf(stderr, "hls: invalid option -- '%c'\n", arg[i]);
            fprintf(stderr, "Try 'hls --help' for more information.\n");
            return (2);
        }
    return (0);
}

