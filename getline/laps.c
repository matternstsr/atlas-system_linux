#include "laps.h"

Car *car_list = NULL;

/**
 * compare_cars - Function to compare two cars by their IDs
 * @a: Pointer to the first car
 * @b: Pointer to the second car
 * 
 * Returns: Difference in IDs
 */
int compare_cars(const void *a, const void *b) {
    const Car *car1 = *(const Car **)a;
    const Car *car2 = *(const Car **)b;
    return car1->id - car2->id;
}

/**
 * free_memory - Function to free the list of cars
 */
void free_memory() {
    Car *current = car_list;
    while (current != NULL) {
        Car *temp = current;
        current = current->next;
        free(temp);
    }
    car_list = NULL;
}

/**
 * update_race_state - Function to update the race state based on the provided IDs
 * @id: Array of car IDs
 * @size: Size of the ID array
 */
void update_race_state(int *id, size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        int current_id = id[i];
        Car *current_car = car_list;
        Car *prev_car = NULL;
        int found = 0;

        while (current_car != NULL) {
            if (current_car->id == current_id) {
                found = 1;
                current_car->laps++;
                break;
            }
            prev_car = current_car;
            current_car = current_car->next;
        }

        if (!found) {
            Car *new_car = malloc(sizeof(Car));
            new_car->id = current_id;
            new_car->laps = 0;
            new_car->next = NULL;

            if (prev_car == NULL) {
                car_list = new_car;
            } else {
                prev_car->next = new_car;
            }

            printf("Car %d joined the race\n", current_id);
        }
    }
}

/**
 * sort_cars - Function to sort the cars by their IDs and display the race state
 */
void sort_cars() {
    int num_cars = 0;
    Car *current = car_list;
    Car **car_array;
    int i;

    while (current != NULL) {
        num_cars++;
        current = current->next;
    }

    car_array = (Car **)malloc(num_cars * sizeof(Car *));
    if (car_array == NULL) {
        /* Memory allocation failed */
        return;
    }

    current = car_list;
    for (i = 0; i < num_cars; i++) {
        car_array[i] = current;
        current = current->next;
    }

    qsort(car_array, num_cars, sizeof(Car *), compare_cars);

    printf("Race state:\n");
    for (i = 0; i < num_cars; i++) {
        printf("Car %d [%d laps]\n", car_array[i]->id, car_array[i]->laps);
    }

    free(car_array);
}



/**
 * display_race_state - Function to display the race state
 */
void display_race_state() {
    sort_cars();
}

/**
 * race_state - Function to handle race state and memory management
 * @id: Array of car IDs
 * @size: Size of the ID array
 */
void race_state(int *id, size_t size) {
    if (size == 0) {
        free_memory();
        return;
    }

    update_race_state(id, size);
    display_race_state();
}
