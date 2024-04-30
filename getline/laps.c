	#include "laps.h"

	/**
	 * compare_cars - Compare function for sorting cars by ID
	 * @a: Pointer to first Car pointer
	 * @b: Pointer to second Car pointer
	 * Return: Difference between IDs of the two cars
	 */
	int compare_cars(const void *a, const void *b)
	{
	const cars_t *car1 = *(const cars_t **)a;
	const cars_t *car2 = *(const cars_t **)b;

	return (car1->id - car2->id);
	}

	/**
	 * free_memory - Free memory allocated for car list
	 * @cars: Pointer to the pointer of the car list
	 */
	void free_memory(cars_t **cars)
	{
	cars_t *current = *cars;
	cars_t *temp;

	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	*cars = NULL;
	}

	/**
	 * sort_cars - Sort the cars in the race by ID
	 * @cars: Pointer to the pointer of the car list
	 */
	void sort_cars(cars_t **cars)
	{
	cars_t *sorted = NULL;
	cars_t *current = *cars;
	cars_t *next;
	cars_t *temp;

	while (current != NULL)
	{
		next = current->next;

		if (sorted == NULL || sorted->id >= current->id)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			temp = sorted;
			while (temp->next != NULL && temp->next->id < current->id) 
				temp = temp->next;
			current->next = temp->next;
			temp->next = current;
		}

		current = next;
	}

	*cars = sorted;
	}

	/**
	 * race_state - Update and display the race state
	 * @id: Array of car IDs to update race state with
	 * @size: Size of the id array
	 * @cars: Pointer to the pointer of the car list
	 */
	void race_state(int *id, size_t size)
	{
	static cars_t *cars, *current, *prev_car, *new_car;
	int found;
	size_t i;

	if (size == 0)
	{
		free_memory(&cars);
		return;
	}
	for (i = 0; i < size; i++)
	{
		current = cars;
		prev_car = NULL;
		found = 0;
		while (current != NULL)
		{
			if (current->id == id[i])
			{
				found = 1;
				current->laps++;
				break;
			}
			prev_car = current, current = current->next;
		}
		if (!found)
		{
			new_car = (cars_t *)malloc(sizeof(cars_t)), new_car->id = id[i];
			new_car->laps = 0, new_car->next = NULL;
			if (prev_car == NULL)
				cars = new_car;
			else
				prev_car->next = new_car;
			printf("Car %d joined the race\n", id[i]);
		}
	}
	sort_cars(&cars);
	printf("Race state:\n");
	current = cars;
	while (current != NULL)
	{
		printf("Car %d [%d laps]\n", current->id, current->laps);
		current = current->next;
	}
	}
