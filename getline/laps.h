#ifndef LAPS_H
#define LAPS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct Car {
    int id;
    int laps;
    struct Car *next;
} Car;

void race_state(int *id, size_t size);

#endif /* LAPS_H */
