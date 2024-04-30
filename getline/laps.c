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
    // Convert linked list to array
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

    // Sort array based on car IDs
    for (int i = 0; i < num_cars - 1; i++) {
        for (int j = 0; j < num_cars - i - 1; j++) {
            if (car_array[j]->id > car_array[j + 1]->id) {
                // Swap nodes
                Car *temp = car_array[j];
                car_array[j] = car_array[j + 1];
                car_array[j + 1] = temp;
            }
        }
    }

    // Update linked list based on sorted array
    car_list = car_array[0];
    current = car_list;
    for (int i = 1; i < num_cars; i++) {
        current->next = car_array[i];
        current = current->next;
    }
    current->next = NULL;
}

void display_race_state() {
    sort_cars(); // Call sort_cars() before displaying race state
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
