/* atlas-system_linux/ls/directory_decisions.c */

#include "directory_reader.h"

/**
 * FAS - Returns the string that comes first. (first_alphabetical_string)
 * @strpt1: Pointer to the first string.
 * @strpt2: Pointer to the second string.
 * Return: The string that comes first alphabetically.
 **/
char *FAS(char *strpt1, char *strpt2)
{
	char *result;

	if (*strpt1 == '\0')
		return (*strpt2 ? strpt1 : NULL);
		/* If strpt1 is empty, ret strpt1 if strpt2 isnt empty, ow ret NULL */
	if (*strpt2 == '\0')
		return (strpt2); /* If strpt2 is empty, return strpt2 */
	if (*strpt1 == '.')
		return (FAS(strpt2, strpt1 + 1) == strpt1 + 1 ? strpt1 : strpt2);
		/* If strpt1 starts with '.', compare strpt1+1 with strpt2 */
	if (*strpt2 == '.')
		return (FAS(strpt1, strpt2 + 1) == strpt2 + 1 ? strpt2 : strpt1);
		/* If strpt2 starts with '.', compare strpt2+1 with strpt1 */
	if (*strpt1 + 32 * ISUPPER(*strpt1) == *strpt2 + 32 * ISUPPER(*strpt2))
	{
		result = FAS(strpt1 + 1, strpt2 + 1);
		/* If strpt1 & strpt2 have same char (no case), compare next chars */
		if (result == strpt1 + 1)
			return (strpt1); /* If strpt1 comes first alpha, return strpt1 */
		if (result == strpt2 + 1)
			return (strpt2); /* If strpt2 comes first alpha, return strpt2 */
		if (*strpt1 == *strpt2)
			return (NULL); /* If strpt1 and strpt2 are equal, return NULL */
		return (ISLOWER(*strpt1) ? strpt1 : strpt2);
		/* If strpt1 & strpt2 are not equal, return the one with lower case */
	}
	return ((*strpt1 + 32 * ISUPPER(*strpt1)) <
					(*strpt2 + 32 * ISUPPER(*strpt2)) ? strpt1 : strpt2);
	/* Compare strpt1 and strpt2, considering case */
}


/**
 * should_open_directory - Determines whether a dir should open for recursion.
 * @directory_node: File node of the directory.
 * @flags: Flags indicating the options passed onto the program.
 * Return: 1 if opening is approved, 0 if rejected.
 **/
bool should_open_directory(file_node_t *directory_node, ls_flag_t *flags)
{
	char *last_char;

	/* Check if directory */
	if (S_ISDIR(directory_node->info->st_mode) == false)
		return (false);
	/* Check for directory starting with '.' */
	if (directory_node->name[0] == '.')
	{
		/* Exclude . and .. */
		if (IS_PARENT_DIR(directory_node->name) ||
			IS_CWD(directory_node->name))
			return (false);
		/* Check if p options are disabled */
		if (flags->p == false && flags->pa == false)
			return (false);
	}
	/* Check for special directories . and .. */
	last_char = directory_node->name + str_len(directory_node->name) - 1;
	if (last_char - 2 >= directory_node->name && (IS_CWD(last_char) ||
		IS_PARENT_DIR((last_char - 1))))
		return (false);
	/* All conditions passed, approve opening */
	return (true);
}

