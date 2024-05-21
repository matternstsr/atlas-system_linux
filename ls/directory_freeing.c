/* atlas-system_linux/ls/directory_freeing.c */

#include "directory_reader.h"

/**
 * free_dir_files - Frees the linked list of files in the directory.
 * @file_list: The head of the file list to be freed.
 **/
void free_dir_files(file_node_t *file_list)
{
	file_node_t *current = file_list, *prev;
	while (current)
	{
		prev = current;
		current = current->next;
		free(prev->info); /* Freeing memory allocated for file information */
		free(prev->name); /* Freeing memory allocated for file name */
		free(prev->dir_name); /* Freeing memory allocated for directory name */
		free(prev); /* Freeing memory allocated for file node */
	}
}


/**
 * free_it_all - Frees all memory allocated for directories and files.
 * @d_head: The head of the directory list.
 * @f_head: The head of the file list.
 **/
void free_it_all(dir_node_t *d_head, file_node_t *f_head)
{
	file_node_t *f; /* Initialize a file node pointer */
	dir_node_t *prev; /* Initialize a directory node pointer */
	if (f_head) /* Check if file list is not empty */
		free_dir_files(f_head); /* Free memory for files */
	while (d_head) /* Loop through directory list */
	{
		f = d_head->list; /* Assign file list of current directory */
		free_dir_files(f); /* Free memory for files in directory */
		prev = d_head; /* Assign current directory to prev */
		d_head = d_head->next; /* Move to next directory */
		free(prev->dir_name); /* Free memory for directory name */
		free(prev); /* Free memory for directory node */
	}
}
