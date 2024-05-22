/* atlas-system_linux/ls/directory_printers.c */

#include "directory_reader.h"

/**
 * print_short_list - Prints a list of file names in short format.
 * @file_list: The list of file nodes to be printed.
 * @flags: The configuration flags.
 **/
void print_short_list(file_node_t *file_list, ls_flag_t *flags)
{
	/* Determine line separator based on configuration */
	char *line_separator = flags->one_per_line ? "\n" : "  ";
	file_node_t *next_node;
	bool first_file = true;
	/* Determine if sorting should be in reverse order */
	bool reverse_order = flags->reversed && (flags->sort_by_size ||  flags->sort_by_time);
	/* If the file list is empty, return */
	if (file_list == NULL)
		return;
	/* Set the initial position based on sort order */
	if (reverse_order)
	{
		while (file_list->next)
			file_list = file_list->next;
		next_node = file_list->prev;
	}
	else
		next_node = file_list->next;
	/* Iterate through the file list */
	while (file_list != NULL)
	{
		/* Determine the next node based on sort order */
		next_node = reverse_order ? file_list->prev : file_list->next;
		/* Print the file name if it should be printed */
		if (should_print(file_list->name) == true)
		{
			printf("%s%s", first_file ? "" : line_separator, file_list->name);
			first_file = false;
		}
		file_list = next_node;
	}
	putchar('\n');
}

/**
 * print_long_list - Prints a list of files in long format (ls -l).
 * @file_list: The list of file nodes to be printed.
 * @flags: The configuration flags.
 **/
void print_long_list(file_node_t *file_list, ls_flag_t *flags)
{
	char perms[11], time[14], user[256], group[256], name[256], buf[256];
	char sym_link_path[256];
	char *str = "%s %u %s %s %u %s %s"; /* Format string for printing */
	unsigned long num_links, size, i;

	if (file_list == NULL) /* Check if the file list is empty */
		return;
	if (flags->reversed) /* Check if printing should be reversed */
		while (file_list->next) /* go to the last node if printing is reversed */
			file_list = file_list->next;
	while (file_list != NULL) /* Iterate through the file list */
	{
		if (should_print(file_list->name) == true) /* Check if file should print */
		{
			perm_log(perms, file_list->info->st_mode);/* Get and log file perms */
			last_modified(time, file_list->info->st_mtime);
			/* Get and log last modified time */
			user_grabber(user, file_list->info->st_uid); /* Get and log user */
			group_grabber(group, file_list->info->st_gid); /* Get and log group */
			string_copy(name, file_list->name); /* Copy file name */
			num_links = file_list->info->st_nlink; /* Get number of links */
			size = file_list->info->st_size; /* Get file size */
			printf(str, perms, num_links, user, group, size, time, name);
			/* Print file info */
			if (S_ISLNK(file_list->info->st_mode) == true)
			/* Check if it's a symbolic link */
			{
				for (i = 0; i < 256; i++) /* Initialize symbolic link path buffer */
					sym_link_path[i] = '\0';
				 /* Construct path to the symbolic link */
				sprintf(buf, "%s/%s", file_list->dir_name, file_list->name);
				readlink(buf, sym_link_path, 256); /* Read symbolic link path */
				printf(" -> %s", sym_link_path); /* Print symbolic link path */
			}
			putchar('\n'); /* Newline after printing file info */
		}
		file_list = flags->reversed ? file_list->prev : file_list->next;
		/* Move to the next or previous node based on printing direction */
	}
}


/**
 * print_dirs - Prints directories.
 * @head: The head of the directory list.
 * @flags: The configuration flags.
 * @printer: The function used to print the directory contents.
 * Return: The status.
 **/
int print_dirs(dir_node_t **head, ls_flag_t *flags, print_t printer)
{
	/* Initialize variables */
	dir_node_t *current_directory = *head;
	int status = 0;

	/* Sort by size if flag is set */
	if (flags->sort_by_size)
		*head = sort_dir_size(*head);
	/* Move to the last directory if reversed flag is set */
	if (flags->reversed)
		while (current_directory->next)
			current_directory = current_directory->next;
	/* Iterate through directories */
	for (; current_directory; current_directory =
			flags->reversed ? current_directory->prev : current_directory->next)
	{
		/* Handle directory errors */
		if (current_directory->error_code)
		{
			errno = current_directory->error_code;
			status = error_message_printing(current_directory->dir_name,
																			program_name);
		}
		else
		{
			if (flags->recursive) /* Manage subdirs if recur flag is set */
				manage_subdirectories(head, current_directory, flags);
			if (flags->print_dir_name) /* Print directory name if flag is set */
				printf("%s:\n", current_directory->dir_name);
			if (flags->sort_by_size) /* Sort by size if flag */
				current_directory->list = file_size_sort(current_directory->list);
			if (flags->sort_by_time) /* Sort by time if flag is set */
				current_directory->list = recent_file_sort(current_directory->list);
			/* Print directory contents */
			printer(current_directory->list, flags);
			if (flags->reversed ? current_directory->prev : current_directory->next)
				putchar('\n'); /* Print newline if there are more directories */
		}
	}
	return (status);
}
