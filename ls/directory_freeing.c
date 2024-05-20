#include "directory_reader.h"

/**
 * Free_Directory_Files - Frees the linked list of files in the directory.
 * @file_list: The head of the file list to be freed.
 **/
void Free_Directory_Files(file_node_t *file_list)
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
 * Free_It_All - Frees all memory allocated for directories and files.
 * @d_head: The head of the directory list.
 * @f_head: The head of the file list.
 **/
void Free_It_All(dir_ops_t *d_head, file_node_t *f_head)
{
	file_node_t *f;
	dir_ops_t *prev;
	if (f_head)
		Free_Directory_Files(f_head);
	while (d_head)
	{
		f = d_head->list;
		Free_Directory_Files(f);
		prev = d_head;
		d_head = d_head->next;
		free(prev->dir_name);
		free(prev);
	}
}
