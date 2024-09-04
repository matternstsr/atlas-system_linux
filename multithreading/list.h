#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>  /* For uintptr_t */
#include <pthread.h>

/* Enum for task status */
typedef enum {
    PENDING,
    STARTED,
    SUCCESS,
    FAILURE
} task_status_t;

/* Function pointer type for task entry function */
typedef void *(*task_entry_t)(void *);

/* Task structure */
typedef struct task_s {
    task_entry_t entry;
    void *param;
    task_status_t status;
    void *result;
    pthread_mutex_t lock;
} task_t;

/* Function prototypes */
task_t *create_task(task_entry_t entry, void *param);
void destroy_task(task_t *task);
void *exec_tasks(const list_t *tasks);

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
