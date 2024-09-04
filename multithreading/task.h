#ifndef TASK_H
#define TASK_H

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

#endif /* TASK_H */
