#include "laps.h"

Car *car_list = NULL;

int compare_cars(const void *a, const void *b) {
    const Car *car1 = *(const Car **)a;
    const Car *car2 = *(const Car **)b;
    return car1->id - car2->id;
}

void free_memory() {
    Car *current = car_list;
    Car *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    car_list = NULL;
}

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
            Car *new_car = (Car *)malloc(sizeof(Car));
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

void sort_cars() {
    /* Not using qsort due to restrictions
       Sorting linked list based on identifiers */
    Car *current = car_list;
    Car *next_car;
    int swapped;

    if (car_list == NULL || car_list->next == NULL) {
        /* No need to sort if there are 0 or 1 cars */
        return;
    }

    do {
        swapped = 0;
        current = car_list;
        next_car = current->next;
        while (next_car != NULL) {
            if (current->id > next_car->id) {
                /* Swap IDs */
                int temp_id = current->id;
                current->id = next_car->id;
                next_car->id = temp_id;
                /* Swap laps */
                int temp_laps = current->laps;
                current->laps = next_car->laps;
                next_car->laps = temp_laps;
                swapped = 1;
            }
            current = next_car;
            next_car = next_car->next;
        }
    } while (swapped);
}

void display_race_state() {
    Car *current = car_list;
    printf("Race state:\n");
    while (current != NULL) {
        printf("Car %d [%d laps]\n", current->id, current->laps);
        current = current->next;
    }
}

void race_state(int *id, size_t size) {
    if (size == 0) {
        free_memory();
        return;
    }

    update_race_state(id, size);
    display_race_state();
}
