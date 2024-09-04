#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/* Create a list with an initial capacity */
list_t *list_create(size_t initial_capacity)
{
    list_t *list = malloc(sizeof(list_t));
    if (!list)
        return NULL;

    list->data = malloc(initial_capacity * sizeof(void *));
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
void list_add(list_t *list, void *value)
{
    if (list->size >= list->capacity)
    {
        size_t new_capacity = list->capacity * 2;
        void **new_data = realloc(list->data, new_capacity * sizeof(void *));
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

/* Destroy the list and free its contents */
void list_destroy(list_t *list, void (*free_func)(void *))
{
    if (list)
    {
        for (size_t i = 0; i < list->size; i++)
        {
            free_func(list->data[i]);
        }
        list_free(list);
    }
}
