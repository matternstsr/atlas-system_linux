/* atlas-system_linux/ls/directory_operations.c */

#include "directory_reader.h"

/**
 * add_dir - Adds a directory node to a directory_node_t linked list.
 * @directory_name: The name of the directory.
 * @directory_stream: The directory stream.
 * @head_of_directory_list: The head of the dir_node_t is where node be added.
 * Return: 0 on success, or an error code if the dir stream cant be opened.
 **/
int add_dir(char *directory_name, DIR *directory_stream,
						dir_node_t **head_of_directory_list)
						{
	dir_node_t *new_directory_node, *current_node;
	struct dirent *entry;
	file_node_t *file_list_head = NULL;
	int error_code = errno;

	/* Allocate memory for new directory node */
	new_directory_node = malloc(sizeof(dir_node_t));
	/* Copy directory name to the new directory node */
	new_directory_node->dir_name = string_dup(directory_name);
	new_directory_node->next = NULL;
	new_directory_node->prev = NULL;
	new_directory_node->size = -1;
	new_directory_node->error_code = directory_stream ? 0 : error_code;
	/* If dir stream is valid, iterate through dir entries and add files */
	if (new_directory_node->error_code == 0)
	{
		while ((entry = readdir(directory_stream)) != NULL)
			error_code = add_file(entry->d_name, directory_name, &file_list_head);
		closedir(directory_stream);
	} /* Set file list of the new directory node */
	new_directory_node->list = file_list_head;
	/* If directory list is empty, set new node as head and return */
	if (*head_of_directory_list == NULL)
	{
		*head_of_directory_list = new_directory_node;
		return (new_directory_node->error_code);
	} /* Find the correct position to putnew dir node in alpha order */
	current_node = *head_of_directory_list;
	while (current_node && FAS(directory_name, current_node->dir_name)
				!= directory_name)
	{
		new_directory_node->prev = current_node;
		current_node = current_node->next;
	}
	if (current_node) /* Insert the new directory node at the correct position */
	{
		new_directory_node->prev = current_node->prev;
		if (current_node->prev)
			current_node->prev->next = new_directory_node;
		new_directory_node->next = current_node;
		current_node->prev = new_directory_node;
		if (current_node == *head_of_directory_list)
			*head_of_directory_list = new_directory_node;
	}
	else
		new_directory_node->prev->next = new_directory_node;
	return (new_directory_node->error_code);
}


/**
 * add_file - Adds a file node to a file_node_t linked list.
 * @file_name: The name of the file.
 * @dir_name: The name of the directory containing the file.
 * @head: The head of file_node_t list where the new node will be added.
 * Return: 0 on success, or an error code if lstat fails.
 **/
int add_file(char *file_name, char *dir_name, file_node_t **head)
{ /* Allocate memory for storing file information */
	file_node_t *new_file_node, *tmp_node;
	struct stat *file_info = malloc(sizeof(struct stat));
	char file_path[512];
	int i, j;

	for (i = 0; dir_name[i]; i++) /* Construct the file path */
		file_path[i] = dir_name[i];
	if (i)
		file_path[i++] = '/';
	for (j = 0; file_name[j]; i++, j++)
		file_path[i] = file_name[j];
	file_path[i] = '\0';
	if (lstat(file_path, file_info) == -1) /* Check if lstat fails */
	{
		free(file_info);
		return (error_message_printing(file_name, program_name));
	} /* Create a new file node */
	new_file_node = file_maker(file_name, dir_name, file_info);
	/* If the list is empty, make the new node the head */
	if (*head == NULL)
	{
		*head = new_file_node;
		return (0);
	} /* Otherwise, find the correct position to insert the new node */
	tmp_node = *head;
	while (tmp_node && FAS(file_name, tmp_node->name) != file_name)
		new_file_node->prev = tmp_node, tmp_node = tmp_node->next;
	if (tmp_node)
	{
		new_file_node->prev = tmp_node->prev;
		if (tmp_node->prev)
			tmp_node->prev->next = new_file_node;
		new_file_node->next = tmp_node;
		tmp_node->prev = new_file_node;
		if (tmp_node == *head)
			*head = new_file_node;
	}
	else
		new_file_node->prev->next = new_file_node;
	return (0);
}


/**
 * file_maker - Initializes a file node.
 * @name: The name of the file.
 * @dir_name: The name of the directory containing the file.
 * @info: The information about the file.
 * Return: Pointer to the newly created file node.
 **/
file_node_t *file_maker(char *name, char *dir_name, struct stat *info)
{
	file_node_t *new_node; /* Allocate memory for a new file node */

	new_node = malloc(sizeof(file_node_t));
	new_node->name = string_dup(name); /* Copy the name of the file */
	new_node->dir_name = string_dup(dir_name); /* Copy the name of the dir */
	new_node->info = info; /* Store the information about the file */
	new_node->next = NULL; /* Initialize next and previous pointers */
	new_node->prev = NULL;
	return (new_node); /* Return the newly created file node */
}


/**
 * manage_subdirs - Lists directory recursively.
 * @head: Head of directory list.
 * @dir: Directory of interest.
 * @flags: Options inputted into the program.
 **/
void manage_subdirs(dir_node_t **head, dir_node_t *dir, ls_flag_t *flags)
{ /* Handle directory traversal in both forward and reverse directions */
	dir_node_t *other_dir, *alt_dir;

	if (flags->rev) /* Check if traversal direction is rev */
	{ /* Save the previous directory and update it with subdirectories */
		other_dir = dir->prev, dir->prev = add_subdirectories(dir, flags);
		/* Check if subdirectories were successfully added */
		if (dir->prev)
		{ /* Set flag to print directory name and update links */
			flags->pdn = true, dir->prev->next = dir;
			/* Traverse to the beginning of the list */
			for (alt_dir = dir->prev; alt_dir->prev; alt_dir = alt_dir->prev)
			alt_dir->prev = other_dir; /* Adjust links to include new directory */
			if (other_dir)
				other_dir->next = alt_dir;
			else
				*head = alt_dir;
		}
		else /* Revert changes if subdirectories couldn't be added */
			dir->prev = other_dir;
	}
	else
	{/* Save the next directory and update it with subdirectories */
		other_dir = dir->next, dir->next = add_subdirectories(dir, flags);
		if (dir->next) /* Check if subdirectories were successfully added */
		{ /* Set flag to print directory name and update links */
			flags->pdn = true, dir->next->prev = dir;
			/* Traverse to the end of the list */
			for (alt_dir = dir->next; alt_dir->next; alt_dir = alt_dir->next)
			alt_dir->next = other_dir; /* Adjust links to include new directory */
			if (other_dir)
				other_dir->prev = alt_dir;
		}
		else
		{ /* Revert changes if subdirectories couldn't be added */
			dir->next = other_dir;
		}
	}
}

/**
 * add_subdirectories - Includes a dirs subdirs in dir list.
 * @dir: Directory node.
 * @flags: Flags (options) passed onto program.
 * Return: Pointer to the directory node.
 **/
dir_node_t *add_subdirectories(dir_node_t *dir, ls_flag_t *flags)
{
	file_node_t *tmp_file = dir->list;
	dir_node_t *new_dir = NULL, *prev_dir = NULL;
	char path[512];

	if (flags->rev) /* Check if the 'rev' flag is set */
		while (tmp_file->next)
		tmp_file = tmp_file->next; /* Iterate dir files */
	for (; tmp_file; tmp_file = flags->rev
				? tmp_file->prev : tmp_file->next)
		if (should_open_directory(tmp_file, flags))
		{ /* Construct full path of the subdirectory */
			sprintf(path, "%s/%s", dir->dir_name, tmp_file->name);
			add_dir(path, opendir(path), &new_dir); /* Add the subdir to the list */
			if (flags->rev) /* Adjust pointers based on the 'rev' flag */
			{
				new_dir->next = prev_dir;
				if (prev_dir)
					prev_dir->prev = new_dir;
			}
			else
			{
				new_dir->prev = prev_dir;
				if (prev_dir)
				prev_dir->next = new_dir;
			}
			prev_dir = new_dir, new_dir = NULL;
		} /* Move 'prev_dir' to the last directory if 'rev' flag is set */
	if (flags->rev && prev_dir)
		while (prev_dir->next)
			prev_dir = prev_dir->next;
	else if (prev_dir) /* Move 'prev_dir' to first dir if 'rev' flag isnt set */
		while (prev_dir->prev)
			prev_dir = prev_dir->prev;
	if (flags->sbs) /* Sort the dir list by size if flag is set */
		prev_dir = sort_dir_size(prev_dir);
	return (prev_dir);
}
