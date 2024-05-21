/* atlas-system_linux/ls/directory_file_sorting.c */

#include "directory_reader.h"

/**
 * recent_file_sort - Sorts file nodes by the time of most recent modification.
 * @head: Pointer to the head of the file node list.
 * Return: Pointer to the head of the sorted list.
 **/
file_node_t *recent_file_sort(file_node_t *head)
{
	file_node_t *current_node = head;

	if (!head || !head->next) /* If the list is empty or has only one node, it's already sorted */
		return head;
	while (current_node->next) /* Traverse the list */
	{
		if (compare_recent(current_node, current_node->next)) /* If current node is more recent than the next one, swap */
		{
			if (current_node == head) /* If current node is the head, update head */
				head = current_node->next;
			swap_files(current_node, current_node->next); /* Swap the nodes */
		}
		else
			current_node = current_node->next; /* Move to the next node */
	}
	for (current_node = head; current_node->next; current_node = current_node->next) /* Check for unsorted pairs */
		if (compare_recent(current_node, current_node->next))
			return recent_file_sort(head); /* If unsorted pair found, re-sort */
	return head; /* Return the sorted list */
}


/**
 * swap_files - Swaps two file nodes.
 * @a: Pointer to the first file node.
 * @b: Pointer to the second file node.
 **/
void swap_files(file_node_t *a, file_node_t *b)
{
	/* Swap the previous node's next pointer */
	if (a->prev)
		a->prev->next = b;
	
	/* Update b's previous pointer */
	b->prev = a->prev;
	
	/* Update a's next pointer */
	a->next = b->next;
	
	/* Swap the next node's previous pointer */
	if (b->next)
		b->next->prev = a;
	
	/* Update b's next pointer */
	b->next = a;
	
	/* Update a's previous pointer */
	a->prev = b;
}


/**
 * compare_recent - Compares file nodes by the time of most recent modification.
 * @a: Pointer to the first file node.
 * @b: Pointer to the second file node.
 * Return: 1 if 'a' is more recent, 0 otherwise.
 **/
int compare_recent(file_node_t *a, file_node_t *b)
{
	time_t a_time = a->info->st_mtime; /* Store the modification time of file 'a' */
	time_t b_time = b->info->st_mtime; /* Store the modification time of file 'b' */
	time_t a_nano = a->info->st_mtim.tv_nsec; /* Store the nanoseconds part of the modification time of file 'a' */
	time_t b_nano = b->info->st_mtim.tv_nsec; /* Store the nanoseconds part of the modification time of file 'b' */

	/* Check if both files have the same modification time and nanoseconds */
	if (a_time == b_time && a_nano == b_nano)
		return (first_alphabetical_string(a->name, b->name) != a->name); /* If same time, return the result of comparing their names alphabetically */
	
	/* If 'a' has a later modification time or the same modification time but later nanoseconds, return 1; otherwise, return 0 */
	return (a_time < b_time || (a_time == b_time && a_nano < b_nano));
}


/**
 * file_size_sort - Sorts a file_node_t linked list by file size.
 * @head: Pointer to the head of the linked list.
 * Return: Pointer to the head of the sorted list.
 **/
file_node_t *file_size_sort(file_node_t *head)
{
    file_node_t *current_node, *next_node, *temporary_node;
    size_t current_size, next_size;
    char *current_name, *next_name;

    if (!head || !head->next)
        return head;
    for (current_node = head, next_node = head->next; next_node; next_node = next_node->next)
    {
        /* Store current node's size and name */
        current_size = current_node->info->st_size;
        next_size = next_node->info->st_size;
        current_name = current_node->name;
        next_name = next_node->name;
        /* Compare sizes and names for sorting */
        if (current_size > next_size || (current_size == next_size && first_alphabetical_string(current_name, next_name) == current_name))
        {
            /* Swap nodes */
            if (current_node->next != next_node)
            {
                if (current_node->prev)
                    current_node->prev->next = current_node->next;
                else
                    head = current_node->next;
                current_node->next->prev = current_node->prev;
                current_node->next = next_node;
                current_node->prev = next_node->prev;
                next_node->prev->next = current_node;
                next_node->prev = current_node;
            }
            current_node = next_node;
        }
        else if ((!next_node->next && (current_size < next_size || first_alphabetical_string(current_name, next_name) == next_name)))
        {
            /* Move current node to the end */
            if (current_node->prev)
                current_node->prev->next = current_node->next;
            else
                head = current_node->next;
            current_node->next->prev = current_node->prev;
            temporary_node = current_node->next;
            current_node->next = NULL;
            current_node->prev = next_node;
            next_node->next = current_node;
            current_node = temporary_node;
            next_node = current_node;
        }
    }
    return file_size_sort_checker(head);
}



/**
 * file_size_sort_checker - Helper function for file_size_sort.
 * @head: Pointer to the head of the list.
 * Return: Pointer to the head of the list.
 **/
file_node_t *file_size_sort_checker(file_node_t *head)
{
	/* Initialize a pointer to traverse the list */
	file_node_t *current_node;

	/* Loop through the list */
	for (current_node = head; current_node->next; current_node = current_node->next)
	{
		/* If current size is greater than next size, continue */
		if (current_node->info->st_size > current_node->next->info->st_size)
			continue;
		/* If current size is smaller than next size, return sorted list */
		if (current_node->info->st_size < current_node->next->info->st_size)
			return file_size_sort(head);
		/* If names are not in alphabetical order, return sorted list */
		if (first_alphabetical_string(current_node->name, current_node->next->name) != current_node->name)
			return file_size_sort(head);
	}
	/* Return the head of the list */
	return head;
}

