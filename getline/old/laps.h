#ifndef LAPS_H
#define LAPS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/**
 * struct cars - Represents a car participating in the race
 * @id: The unique identifier of the car
 * @laps: The number of laps completed by the car
 * @next: Pointer to the next car in the linked list
 */
typedef struct cars
{
	int id;
	int laps;
	struct cars *next;
} cars_t;

void race_state(int *id, size_t size);

#endif /* LAPS_H */
