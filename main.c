#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Description of the task
// https://www.spoj.com/problems/AGGRCOW/

#define MAX_NUM_STALLS (100000)

int cmp_stalls (const void *a, const void *b)
{
   return (*(int*)a - *(int*)b);
}

int place_cows(int num_stalls, int num_cows, uint32_t *p_stall, uint32_t min_distance)
{
    // Try placing all the cows in stalls starting from the stall with the minimal number and
    // ensuring that the distance between any two neighbour cows is greater or equal to the given minimal distance.

    // There are at least 2 cows.
    // The first stall is taken by the first cow. Start from the second stall and the second cow (with index 1).
    int stall_idx;
    int last_taken_stall_idx = 0;
    int cows_placed = 1;
    for (stall_idx = 1; stall_idx < num_stalls && cows_placed < num_cows; stall_idx++)
    {
        // Calculate the distance between the current stall and the stall, where the previous cow was placed
        uint32_t distance = p_stall[stall_idx] - p_stall[last_taken_stall_idx];
        // Place current cow, in case the distance is at least the given minimal distance
        if (distance >= min_distance)
        {
            cows_placed++;
            last_taken_stall_idx = stall_idx;
        }
    }

//    return cows_placed;
    if (cows_placed == num_cows && (stall_idx + 1) == num_stalls)
    {
        return 0; // all cows placed, no stalls left
    }
    else if (cows_placed == num_cows)
    {
        return 1; // all cows placed, some stalls left
    }
    else
    {
        return -1; // not enough stalls to place the cows
    }
}

uint32_t solve(int num_stalls, int num_cows, uint32_t *p_stall)
{
    // Stalls might be unsorted, sort them
    qsort(&p_stall[0], num_stalls, sizeof(p_stall[0]), cmp_stalls);

    uint32_t min = 1;
    uint32_t max = p_stall[num_stalls - 1] - p_stall[0];
    uint32_t mid = (max - min) / 2;


    while (min <= max)
    {
        mid = (max - min) / 2;
        int place_result = place_cows(num_stalls, num_cows, p_stall, mid);
        if (place_result == 1)
        {
            min = mid + 1; // all cows placed, some stalls left
        }
        else if (place_result == -1)
        {
            max = mid - 1; // // not enough stalls to place the cows
        }
        else
        {
            return mid;
        }
    }
    return mid;
}

int main()
{
    int num_stalls;                  // N [2; 100 000]
    int num_cows;                    // C [2; N]
    uint32_t a_stall[MAX_NUM_STALLS]; // Stall positions [x1; xN], where xi [0; 1 000 000 000]

    int num_testcases;
    scanf("%d", &num_testcases);
    int t;
    for (t = 0; t < num_testcases; t++)
    {
        scanf("%d %d", &num_stalls, &num_cows);

        // Read stall positions
        int i, stall_position;
        for (i = 0; i < num_stalls; i++)
        {
            scanf("%d", &stall_position);
            a_stall[i] = stall_position;
        }
        uint32_t result = solve(num_stalls, num_cows, &a_stall[0]);
        printf("%u\n", result);
    }

    return 0;
}
