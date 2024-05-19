#include "directory_reader.h"

/**
 * recent_file_sort - Sorts file nodes by the time of most recent modification.
 * @head: Pointer to the head of the file node list.
 * Return: Pointer to the head of the sorted list.
 **/
file_node_t *recent_file_sort(file_node_t *head)
{
	file_node_t *current_node = head;

	if (!head || !head->next)
		return head;
	while (current_node->next)
	{
		if (compare_recent(current_node, current_node->next))
		{
			if (current_node == head)
				head = current_node->next;
			swap_files(current_node, current_node->next);
		}
		else
			current_node = current_node->next;
	}
	for (current_node = head; current_node->next; current_node = current_node->next)
		if (compare_recent(current_node, current_node->next))
			return recent_file_sort(head);
	return head;
}

/**
 * swap_files - Swaps two file nodes.
 * @a: Pointer to the first file node.
 * @b: Pointer to the second file node.
 **/
void swap_files(file_node_t *a, file_node_t *b)
{
	if (a->prev)
		a->prev->next = b;
	b->prev = a->prev;
	a->next = b->next;
	if (b->next)
		b->next->prev = a;
	b->next = a;
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
	time_t a_time = a->info->st_mtime;
	time_t b_time = b->info->st_mtime;
	time_t a_nano = a->info->st_mtim.tv_nsec;
	time_t b_nano = b->info->st_mtim.tv_nsec;

	if (a_time == b_time && a_nano == b_nano)
		return (first_alphabetical_string(a->name, b->name) != a->name);
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
        current_size = current_node->info->st_size;
        next_size = next_node->info->st_size;
        current_name = current_node->name;
        next_name = next_node->name;
        if (current_size > next_size || (current_size == next_size && first_alphabetical_string(current_name, next_name) == current_name))
        {
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
	file_node_t *current_node;

	for (current_node = head; current_node->next; current_node = current_node->next)
	{
		if (current_node->info->st_size > current_node->next->info->st_size)
			continue;
		if (current_node->info->st_size < current_node->next->info->st_size)
			return file_size_sort(head);
		if (first_alphabetical_string(current_node->name, current_node->next->name) != current_node->name)
			return file_size_sort(head);
	}
	return head;
}
