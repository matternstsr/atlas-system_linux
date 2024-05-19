#include "directory_reader.h"

/**
 * print_short_list - Prints a list of file names in short format.
 * @file_list: The list of file nodes to be printed.
 * @flags: The configuration flags.
 **/
void print_short_list(file_node_t *file_list, ls_flag_t *flags)
{
	char *line_separator = flags->one_per_line ? "\n" : "  ";
	file_node_t *next_node;
	bool first_file = true;
	bool reverse_order = flags->reversed && (flags->sort_by_size || flags->sort_by_time);
	if (file_list == NULL)
		return;
	if (reverse_order)
	{
		while (file_list->next)
			file_list = file_list->next;
		next_node = file_list->prev;
	}
	else
	{
		next_node = file_list->next;
	}
	while (file_list != NULL)
	{
		next_node = reverse_order ? file_list->prev : file_list->next;
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
	char *str = "%s %u %s %s %u %s %s";
	unsigned long num_links, size, i;
	if (file_list == NULL)
		return;
	if (flags->reversed)
		while (file_list->next)
			file_list = file_list->next;
	while (file_list != NULL)
	{
		if (should_print(file_list->name) == true)
		{
			perm_log(perms, file_list->info->st_mode);
			last_modified(time, file_list->info->st_mtime);
			user_grabber(user, file_list->info->st_uid);
			group_grabber(group, file_list->info->st_gid);
			string_copy(name, file_list->name);
			num_links = file_list->info->st_nlink;
			size = file_list->info->st_size;
			printf(str, perms, num_links, user, group, size, time, name);
			if (S_ISLNK(file_list->info->st_mode) == true)
			{
				for (i = 0; i < 256; i++)
					sym_link_path[i] = '\0';
				sprintf(buf, "%s/%s", file_list->dir_name, file_list->name);
				readlink(buf, sym_link_path, 256);
				printf(" -> %s", sym_link_path);
			}
			putchar('\n');
		}
		file_list = flags->reversed ? file_list->prev : file_list->next;
	}
}

/**
 * print_dirs - Prints directories.
 * @head: The head of the directory list.
 * @flags: The configuration flags.
 * @printer: The function used to print the directory contents.
 * Return: The status.
 **/
int print_dirs(dir_ops_t **head, ls_flag_t *flags, print_t printer)
{
	dir_ops_t *current_directory = *head;
	int status = 0;

	if (flags->sort_by_size)
		*head = sort_dir_size(*head);
	if (flags->reversed)
		while (current_directory->next)
			current_directory = current_directory->next;
	for (; current_directory; current_directory = flags->reversed ? current_directory->prev : current_directory->next)
	{
		if (current_directory->error_code)
		{
			errno = current_directory->error_code;
			status = error_message_printing(current_directory->dir_name);
		}
		else
		{
			if (flags->recursive)
				manage_subdirectories(head, current_directory, flags);
			if (flags->print_dir_name)
				printf("%s:\n", current_directory->dir_name);
			if (flags->sort_by_size)
				current_directory->list = file_size_sort(current_directory->list);
			if (flags->sort_by_time)
				current_directory->list = recent_file_sort(current_directory->list);
			printer(current_directory->list, flags);
			if (flags->reversed ? current_directory->prev : current_directory->next)
				putchar('\n');
		}
	}
	return status;
}
