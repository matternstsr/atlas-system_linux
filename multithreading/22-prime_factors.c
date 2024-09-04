#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include "20-tprintf.c"
#include "list.c"
#include "multithreading.h"

/**
* create_task - Create a new task with a specified entry function and parameter.
* @entry: Function pointer to the task entry function.
* @param: Parameter for the entry function.
*
* Return: Pointer to the newly created task structure.
*/
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = malloc(sizeof(task_t));
	if (!task)
		return NULL;

	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);

	return task;
}

/**
* destroy_task - Free resources associated with a task.
* @task: Pointer to the task to destroy.
*
* Cleans up and deallocates the memory for the specified task.
*/
void destroy_task(task_t *task)
{
	if (task)
	{
		pthread_mutex_destroy(&task->lock);
		free(task);
	}
}

/**
 * exec_tasks - Execute a list of tasks in a thread-safe manner.
 * @tasks: List of tasks to execute.
 *
 * Each task is processed by multiple threads, ensuring thread safety.
 *
 * Return: Always returns NULL.
 */
void *exec_tasks(const list_t *tasks)
{
    node_t *current_node;
    task_t *task;

    /* Start from the head of the list */
    current_node = tasks->head;

    while (current_node)
    {
        pthread_mutex_lock(&print_mutex);

        task = (task_t *)current_node->content;

        if (task->status == PENDING)
        {
            task->status = STARTED;
            tprintf("Task started: %p\n", (void *)task);
            pthread_mutex_unlock(&print_mutex);

            /* Execute the task */
            task->result = task->entry(task->param);

            pthread_mutex_lock(&print_mutex);
            task->status = SUCCESS;
            tprintf("Task completed: %p\n", (void *)task);
        }
        else
        {
            pthread_mutex_unlock(&print_mutex);
        }

        pthread_mutex_unlock(&print_mutex);

        /* Move to the next node */
        current_node = current_node->next;
    }

    return NULL;
}
