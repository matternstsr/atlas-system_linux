#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>  /* For uintptr_t */
#include "task.h"

typedef struct list_s
{
    void **data;
    size_t size;
    size_t capacity;
} list_t;

/* Function prototypes */
list_t *list_create(size_t initial_capacity);
void list_add(list_t *list, void *value);
void list_free(list_t *list);
void list_destroy(list_t *list, void (*free_func)(void *));

#endif /* LIST_H */
