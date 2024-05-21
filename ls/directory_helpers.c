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
	if (source_string == NULL)
		return NULL;
	for (length = 0; source_string[length]; length++) /* Get string length */
		;
	length++;
	copied_string = (char *)malloc(length); /* Allocate memory */
	if (copied_string == NULL)		        /* Check for NULL */
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
    if (!str)
        return NULL;
    while (*str)
        if (*str != c)
            str++;
        else
            return str;
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
	if (!dest || !src)
		return (NULL);
	for (i = 0; src[i]; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
	return (dest);
}

/**
* str_len - Returns the length of a string.
* @s: The string to be evaluated.
* Return: An integer representing the length of the string.
**/
int str_len(char *s)
{
	int strlen = 0;
	while (s[strlen])
		strlen++;
	return (strlen);
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
        if (arg[i] == '1')
            flags->one_per_line = true;
        else if (arg[i] == 'a')
            flags->dot = true;
        else if (arg[i] == 'A')
            flags->dot_alt = true;
        else if (arg[i] == 'l')
            flags->printer = &print_long_list;
        else if (arg[i] == 'r')
            flags->reversed = true;
        else if (arg[i] == 'R')
            flags->recursive = true;
        else if (arg[i] == 't')
            flags->sort_by_time = true;
        else if (arg[i] == 'S')
            flags->sort_by_size = true;
        else
        {
            fprintf(stderr, "hls: invalid option -- '%c'\n", arg[i]);
            fprintf(stderr, "Try 'hls --help' for more information.\n");
            return (2);
        }
    return (0);
}
