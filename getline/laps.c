#include "laps.h"

Car *car_list = NULL;

//Function to compare two cars by their IDs
int compare_cars(const void *a, const void *b) {
    const Car *car1 = *(const Car **)a;
    const Car *car2 = *(const Car **)b;
    return car1->id - car2->id;
}

void race_state(int *id, size_t size) {
    if (size == 0) {
        // Free all of the memory that was allocated
        Car *current = car_list;
        while (current != NULL) {
            Car *temp = current;
            current = current->next;
            free(temp);
        }
        car_list = NULL;
        return;
    }

    for (size_t i = 0; i < size; i++) {
        int current_id = id[i];
        Car *current_car = car_list;
        Car *prev_car = NULL;
        int found = 0;

        // Checking if the current car exists in the list of the cars
        while (current_car != NULL) {
            if (current_car->id == current_id) {
                found = 1;
                current_car->laps++;
                break;
            }
            prev_car = current_car;
            current_car = current_car->next;
        }

        // Make a new car if not found
        if (!found) {
            Car *new_car = malloc(sizeof(Car));
            new_car->id = current_id;
            new_car->laps = 0;
            new_car->next = NULL;

            // Adding a new car to the list
            if (prev_car == NULL) {
                car_list = new_car;
            } else {
                prev_car->next = new_car;
            }

            printf("Car %d joined the race\n", current_id);
        }
    }

    // Sort the cars by their IDs
    int num_cars = 0;
    Car *current = car_list;
    while (current != NULL) {
        num_cars++;
        current = current->next;
    }
    Car *car_array[num_cars];
    current = car_list;
    for (int i = 0; i < num_cars; i++) {
        car_array[i] = current;
        current = current->next;
    }
    qsort(car_array, num_cars, sizeof(Car *), compare_cars);

    // Show the state of the race
    printf("Race state:\n");
    for (int i = 0; i < num_cars; i++) {
        printf("Car %d [%d laps]\n", car_array[i]->id, car_array[i]->laps);
    }
}
