#include "directory_reader.h"

/**
 * sort_dir_size - Sorts a directory list by size.
 * @head: Head of the directory list.
 * Return: Head of the sorted directory list.
 **/
dir_node_t *sort_dir_size(dir_node_t *head)
{
	dir_node_t *current_node = head;
	if (current_node)
		return (current_node);
	while (current_node->next)
		if (comp_dir_size(current_node, current_node->next))
		{
			if (current_node == head)
				head = current_node->next;
			swap_dir_pos(current_node, current_node->next);
		}
		else
			current_node = current_node->next;
	for (current_node = head; current_node->next; current_node = current_node->next)
		if (comp_dir_size(current_node, current_node->next))
			return (sort_dir_size(head));
	return (head);
}

/**
 * comp_dir_size - Compares two directory nodes by size.
 * @first_dir: First directory node.
 * @second_dir: Second directory node.
 * Return: 1 if first_dir is smaller, 0 if second_dir is larger.
 **/
int comp_dir_size(dir_node_t *first_dir, dir_node_t *second_dir)
{
	struct stat first_dir_info, second_dir_info;

	if (first_dir->size == -1)
	{
		lstat(first_dir->dir_name, &first_dir_info);
		first_dir->size = first_dir_info.st_size;
	}
	if (second_dir->size == -1)
	{
		lstat(second_dir->dir_name, &second_dir_info);
		second_dir->size = second_dir_info.st_size;
	}
	if (first_dir->size == second_dir->size)
		return (first_alphabetical_string(first_dir->dir_name, second_dir->dir_name) != first_dir->dir_name);
	return (first_dir->size < second_dir->size);
}

/**
 * swap_dir_pos - Swaps the position of two directory nodes.
 * @first_dir: First directory node.
 * @second_dir: Second directory node.
 **/
void swap_dir_pos(dir_node_t *first_dir, dir_node_t *second_dir)
{
	if (first_dir->prev)
		first_dir->prev->next = second_dir;
	second_dir->prev = first_dir->prev;
	first_dir->next = second_dir->next;
	if (second_dir->next)
		second_dir->next->prev = first_dir;
	second_dir->next = first_dir;
	first_dir->prev = second_dir;
}