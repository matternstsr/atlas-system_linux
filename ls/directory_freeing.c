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
		free(prev->info);
		free(prev->name);
		free(prev->dir_name);
		free(prev);
	}
}

/**
 * free_it_all - Frees all memory allocated for directories and files.
 * @d_head: The head of the directory list.
 * @f_head: The head of the file list.
 **/
void free_it_all(dir_node_t *d_head, file_node_t *f_head)
{
	file_node_t *f;
	dir_node_t *prev;
	if (f_head)
		free_dir_files(f_head);
	while (d_head)
	{
		f = d_head->list;
		free_dir_files(f);
		prev = d_head;
		d_head = d_head->next;
		free(prev->dir_name);
		free(prev);
	}
}