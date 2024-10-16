#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include "multithreading.h"

/**
* create_task - Create a new task with a specified entry
* function and parameter.
* @entry: Function pointer to the task entry function.
* @param: Parameter for the entry function.
*
* Return: Pointer to the newly created task structure.
*/

task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = malloc(sizeof(task_t));
	static int next_task_id;

	if (!task)
		return (NULL);

	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);
	task->id = next_task_id++;  /* Assign a unique ID */

	return (task);
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
		list_destroy(task->result, free);
		free(task->result);
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
	pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

	/* Start from the head of the list */
	current_node = tasks->head;

	while (current_node)
	{
		pthread_mutex_lock(&print_mutex);

		task = (task_t *)current_node->content;

		if (task->status == PENDING)
		{
			task->status = STARTED;
			tprintf("[%02d] Started\n", task->id);  /* Use ID with zero-padding */
			pthread_mutex_unlock(&print_mutex);

			/* Execute the task */
			task->result = task->entry(task->param);

			pthread_mutex_lock(&print_mutex);
			task->status = SUCCESS;
			tprintf("[%02d] Success\n", task->id);  /* Use ID with zero-padding */
		}
		else
		{
			pthread_mutex_unlock(&print_mutex);
		}

		pthread_mutex_unlock(&print_mutex);

		/* Move to the next node */
		current_node = current_node->next;
	}

	return (NULL);
}



