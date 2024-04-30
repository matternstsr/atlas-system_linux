#include "laps.h"

Car *car_list = NULL;

/**
 * compare_cars - Compare function for sorting cars by ID
 * @a: Pointer to first Car pointer
 * @b: Pointer to second Car pointer
 * Return: Difference between IDs of the two cars
 */
int compare_cars(const void *a, const void *b)
{
	const Car *car1 = *(const Car **)a;
	const Car *car2 = *(const Car **)b;
	return car1->id - car2->id;
}


/**
 * free_memory - Free memory allocated for car list
 */
void free_memory(void)
{
	Car *current = car_list;
	Car *temp;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	car_list = NULL;
}

/**
 * update_race_state - Update race state with new car IDs
 * @id: Array of car IDs to update race state with
 * @size: Size of the id array
 */
void update_race_state(int *id, size_t size)
{
	size_t i;
	for (i = 0; i < size; i++)
	{
		int current_id = id[i];
		Car *current_car = car_list;
		Car *prev_car = NULL;
		int found = 0;

		while (current_car != NULL)
		{
			if (current_car->id == current_id)
			{
				found = 1;
				current_car->laps++;
				break;
			}
			prev_car = current_car;
			current_car = current_car->next;
		}

		if (!found)
		{
			Car *new_car = (Car *)malloc(sizeof(Car));
			new_car->id = current_id;
			new_car->laps = 0;
			new_car->next = NULL;

			if (prev_car == NULL)
			{
				car_list = new_car;
			} else {
				prev_car->next = new_car;
			}

			printf("Car %d joined the race\n", current_id);
		}
	}
}

/**
 * sort_cars - Sort the cars in the race by ID
 */
void sort_cars(void) 
{
	Car *current = car_list;
	Car *sorted_list = NULL;

	/* Traverse the original list */
	while (current != NULL) 
	{
		Car *next = current->next;

		/* Insert current node into sorted_list */
		if (sorted_list == NULL || current->id < sorted_list->id)
		{
			current->next = sorted_list;
			sorted_list = current;
		} else
		{
			Car *search = sorted_list;
			while (search->next != NULL && current->id > search->next->id)
			{
				search = search->next;
			}
			current->next = search->next;
			search->next = current;
		}

		/* Move to the next node in the original list */
		current = next;
	}

	/* Update car_list with the sorted list */
	car_list = sorted_list;
}

/**
 * display_race_state - Display the current race state
 */
void display_race_state(void) 
{
	Car *current;

	sort_cars(); /* Call sort_cars() before displaying race state */
	printf("Race state:\n");
	current = car_list;
	while (current != NULL) 
	{
		printf("Car %d [%d laps]\n", current->id, current->laps);
		current = current->next;
	}
}

/**
 * race_state - Update and display the race state
 * @id: Array of car IDs to update race state with
 * @size: Size of the id array
 */
void race_state(int *id, size_t size)
{
	if (size == 0)
	{
		free_memory();
		return;
	}

	update_race_state(id, size);
	display_race_state();
}
