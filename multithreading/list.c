#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* Create a list with an initial capacity */
list_t *list_create(size_t initial_capacity)
{
list_t *list = malloc(sizeof(list_t));
if (!list)
	return NULL;

list->data = malloc(initial_capacity * sizeof(unsigned long));
if (!list->data)
{
	free(list);
	return NULL;
}

list->size = 0;
list->capacity = initial_capacity;
return list;
}

/* Add a value to the list */
void list_add(list_t *list, unsigned long value)
{
if (list->size >= list->capacity)
{
	size_t new_capacity = list->capacity * 2;
	unsigned long *new_data = realloc(list->data, new_capacity * sizeof(unsigned long));
	if (!new_data)
		return; /* Handle memory allocation failure */
	list->data = new_data;
	list->capacity = new_capacity;
}

list->data[list->size++] = value;
}

/* Free the list memory */
void list_free(list_t *list)
{
if (list)
{
	free(list->data);
	free(list);
}
}
