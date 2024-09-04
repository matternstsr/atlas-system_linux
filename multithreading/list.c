#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/**
 * node_create - Creates a Node structure and initializes it
 *
 * @content: Address of the custom content to store in the node
 *
 * Return: A pointer to the created node
 */
node_t *node_create(void *content)
{
    node_t *node = malloc(sizeof(*node));
    if (!node) {
        perror("Failed to allocate node");
        exit(EXIT_FAILURE);
    }

    node->content = content;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/**
 * list_add - Creates a Node and adds it to the back of a list
 *
 * @list:    Pointer to the list to add the node to
 * @content: Address of the custom content to store in the node
 *
 * Return: A pointer to the created node
 */
node_t *list_add(list_t *list, void *content)
{
    node_t *node = node_create(content);

    if (!list->tail) {
        list->head = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }
    list->tail = node;
    list->size++;
    return node;
}

/**
 * list_init - Initializes a list structure
 *
 * Return: A pointer to the initialized list
 */
list_t *list_init(void)
{
    list_t *list = malloc(sizeof(list_t));
    if (!list) {
        perror("Failed to allocate list");
        exit(EXIT_FAILURE);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


/**
 * list_destroy - Destroys the content of a list
 *
 * @list:      Pointer to the list structure to destroy the content of
 * @free_func: Pointer to a function to use to free the content of a node
 */
void list_destroy(list_t *list, node_func_t free_func)
{
    node_t *node = list->head;
    while (node)
    {
        node_t *tmp = node;
        node = node->next;
        if (free_func)
            free_func(tmp->content);
        free(tmp);
    }
    free(list);
}

/**
 * list_each - Iterates over a list and calls a function for each node
 *
 * @list: Pointer to the list to iterate over
 * @func: Pointer to a function to call with the content of each node
 */
void list_each(list_t *list, node_func_t func)
{
    node_t *node = list->head;
    while (node)
    {
        func(node->content);
        node = node->next;
    }
}
