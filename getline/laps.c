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
 * new_car - Add a new car to the car list
 * @id: ID of the new car
 * @cars: Pointer to the pointer of the car list
 */
void new_car(int id, cars_t **cars)
{
    cars_t *current = *cars;
    cars_t *prev_car = NULL;
    int found = 0;

    while (current != NULL)
    {
        if (current->id == id)
        {
            found = 1;
            current->laps++;
            break;
        }
        prev_car = current;
        current = current->next;
    }

    if (!found)
    {
        cars_t *new_car = (cars_t *)malloc(sizeof(cars_t));
        new_car->id = id;
        new_car->laps = 0;
        new_car->next = NULL;

        if (prev_car == NULL)
            *cars = new_car;
        else
            prev_car->next = new_car;

        printf("Car %d joined the race\n", id);
    }
}

/**
 * update_race_state - Update race state with new car IDs
 * @id: Array of car IDs to update race state with
 * @size: Size of the id array
 * @cars: Pointer to the pointer of the car list
 */
void update_race_state(int *id, size_t size, cars_t **cars)
{
    size_t i;

    for (i = 0; i < size; i++)
    {
        new_car(id[i], cars);
    }
}

/**
 * sort_cars - Sort the cars in the race by ID
 * @cars: Pointer to the pointer of the car list
 */
void sort_cars(cars_t **cars)
{
    int num_cars = 0;
    cars_t *current = *cars;
    cars_t **car_array;
    int i;

    /* Count the number of cars */
    current = *cars;
    while (current != NULL)
    {
        num_cars++;
        current = current->next;
    }

    /* Allocate memory for the array */
    car_array = (cars_t **)malloc(num_cars * sizeof(cars_t *));
    if (car_array == NULL)
    {
        /* Handle memory allocation failure */
        return;
    }

    /* Populate the array with car pointers */
    current = *cars;
    i = 0;
    for (; i < num_cars && current != NULL; i++)
    {
        car_array[i] = current;
        current = current->next;
    }

    /* Sort the array based on car IDs */
    qsort(car_array, num_cars, sizeof(cars_t *), compare_cars);

    /* Update car list based on sorted array */
    *cars = car_array[0];
    current = *cars;
    for (i = 1; i < num_cars; i++)
    {
        current->next = car_array[i];
        current = current->next;
    }
    current->next = NULL;

    /* Free the temporary array */
    free(car_array);
}

/**
 * display_race_state - Display the current race state
 * @cars: Pointer to the pointer of the car list
 */
void display_race_state(cars_t **cars)
{
	cars_t *current;
	
    sort_cars(cars); /* Call sort_cars() before displaying race state */
    printf("Race state:\n");
    current = *cars;
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
 * @cars: Pointer to the pointer of the car list
 */

void race_state(int *id, size_t size)
{
	static cars_t **cars;
    if (size == 0)
    {
        free_memory(cars);
        return;
    }

    update_race_state(id, size, cars);
    display_race_state(cars);
}
