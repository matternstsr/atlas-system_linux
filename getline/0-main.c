#include "laps.h"

#include "laps.h"

/**
 * main - entry point.
 *
 * Return: always 0.
 */
int main()
{

    int ids[3] = {89, 2, 102};
    int i;
    
    for (i = 0 ; i < 10 ; i ++) 
    {
      race_state(ids, 3);
    }
    race_state(NULL, 0);
    return (0);
}