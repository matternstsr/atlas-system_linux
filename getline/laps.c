#include "laps.h"

Car *car_list = NULL;

/**
 * 
*/
void race_state(int *id, size_t size)
{
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

    // Show the state of the race
    printf("Race state:\n");
    Car *current = car_list;
    while (current != NULL) {
        printf("Car %d [%d laps]\n", current->id, current->laps);
        current = current->next;
    }
}
