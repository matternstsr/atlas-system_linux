/* atlas-system_linux/ls/directory_sorters.c */

#include "directory_reader.h"

/**
 * sort_dir_size - Sorts a directory list by size.
 * @head: Head of the directory list.
 * Return: Head of the sorted directory list.
 **/
dir_node_t *sort_dir_size(dir_node_t *head)
{
	dir_node_t *current_node;
	int swapped;

	/* If the list is empty or has only one element, it's already sorted */
	if (!head || !head->next)
		return head;
	do
	{
		swapped = 0;
		current_node = head;
		/* Traverse the list */
		while (current_node->next)
		{
			/* Compare sizes of adjacent nodes and swap if necessary */
			if (comp_dir_size(current_node, current_node->next))
			{
				/* Update head pointer if current_node is the head */
				if (current_node == head)
					head = current_node->next;
				/* Swap positions of current_node and its next node */
				swap_dir_pos(current_node, current_node->next);
				swapped = 1; /* Mark that a swap has occurred */
			}
			current_node = current_node->next;
		}
	} while (swapped); /* Repeat until no more swaps are needed */
	return head;
}

/**
 * comp_dir_size - Compares two directory nodes by size.
 * @first_dir: Pointer to the first directory node.
 * @second_dir: Pointer to the second directory node.
 * Return: 1 if first_dir is smaller, 0 if second_dir is larger.
 **/
int comp_dir_size(dir_node_t *first_dir, dir_node_t *second_dir) {
struct stat first_dir_info, second_dir_info;

/* Check if either directory node is NULL */
if (!first_dir || !second_dir)
	return 0; /* Indicate they are equal in size since they are not valid for comparison */
/* Retrieve size information if it's not already cached */
if (first_dir->size == -1) {
	if (lstat(first_dir->dir_name, &first_dir_info) == -1)
		return 0; /* Unable to get size, consider them equal */
	first_dir->size = first_dir_info.st_size;
}
if (second_dir->size == -1) {
	if (lstat(second_dir->dir_name, &second_dir_info) == -1)
		return 0; /* Unable to get size, consider them equal */
	second_dir->size = second_dir_info.st_size;
}
/* Compare sizes */
if (first_dir->size == second_dir->size) {
	/* If sizes are equal, compare directory names lexicographically */
	return (first_alphabetical_string(first_dir->dir_name, second_dir->dir_name) != first_dir->dir_name);
} else {
	/* Otherwise, directly compare sizes */
	return (first_dir->size < second_dir->size);
}
}


/**
 * swap_dir_pos - Swaps the position of two directory nodes.
 * @first_dir: Pointer to the first directory node.
 * @second_dir: Pointer to the second directory node.
 **/
void swap_dir_pos(dir_node_t *first_dir, dir_node_t *second_dir) {
	/* Ensure both nodes are valid */
	if (!first_dir || !second_dir)
		return;
	/* Check if the nodes are the same */
	if (first_dir == second_dir)
		return;
	/* Update prev pointers of adjacent nodes */
	if (first_dir->prev)
		first_dir->prev->next = second_dir;
	if (second_dir->next)
		second_dir->next->prev = first_dir;
	/* Update next pointers of adjacent nodes */
	first_dir->next = second_dir->next;
	second_dir->next = first_dir->next;
	/* Update prev pointers of swapped nodes */
	first_dir->prev = second_dir->prev;
	second_dir->prev = first_dir->prev;
	/* Update next pointers of swapped nodes */
	if (first_dir->prev)
		first_dir->prev->next = first_dir;
	if (second_dir->prev)
		second_dir->prev->next = second_dir;
}
