#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct list_s
{
	unsigned long *data;
	size_t size;
	size_t capacity;
} list_t;

/* Function prototypes */
list_t *list_create(size_t initial_capacity);
void list_add(list_t *list, unsigned long value);
void list_free(list_t *list);

#endif /* LIST_H */
