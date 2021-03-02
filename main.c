#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Description of the task
// https://www.spoj.com/problems/AGGRCOW/

#define MAX_NUM_STALLS (100000)

#define INPUT_FROM_FILE (0) // 0: no, 1: yes
#if INPUT_FROM_FILE == (1)
    #define INPUT_FILE "D:\\data\\git\\aggrcows\\input.txt"
    //#define INPUT_FILE "D:\\data\\git\\aggrcows\\test\\testcase.txt"
#endif

int cmp_stalls (const void *a, const void *b)
{
   return (*(int*)a - *(int*)b);
}

uint32_t solve(int num_stalls, int num_cows, uint32_t *p_stall)
{
    // Stalls might be unsorted, sort them
    qsort(&p_stall[0], num_stalls, sizeof(p_stall[0]), cmp_stalls);

    uint32_t min = 1;
    uint32_t max = p_stall[num_stalls - 1] - p_stall[0];
    uint32_t mid;
    uint32_t best_solution = 1;

    while (min <= max)
    {
        mid = min + (max - min) / 2;
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
                if (distance >= mid)
                {
                    cows_placed++;
                    last_taken_stall_idx = stall_idx;
                }
            }

            if (cows_placed == num_cows)
            {
                best_solution = mid;
                min = mid + 1;
            }
            else // if (cows_placed < num_cows)
            {
                max = mid - 1;
            }
        }
    }
    return best_solution;
}

#if INPUT_FROM_FILE == (1)
int main()
{
    FILE * fp;
    fp = fopen(INPUT_FILE, "r");

    if (fp == NULL)
    {
       printf("Error: no input file was found at the given path [%s]\n", INPUT_FILE);
       exit(EXIT_FAILURE);
    }

    int num_stalls;                   // N [2; 100 000]
    int num_cows;                     // C [2; N]
    uint32_t a_stall[MAX_NUM_STALLS]; // Stall positions [x1; xN], where xi [0; 1 000 000 000]

    int num_testcases;
    fscanf(fp, "%d", &num_testcases);
    int t;
    for (t = 0; t < num_testcases; t++)
    {
        fscanf(fp, "%d %d", &num_stalls, &num_cows);

        // Read stall positions
        int i, stall_position;
        for (i = 0; i < num_stalls; i++)
        {
            fscanf(fp, "%d", &stall_position);
            a_stall[i] = stall_position;
        }
        uint32_t result = solve(num_stalls, num_cows, &a_stall[0]);
        printf("%u\n", result);
    }

    fclose(fp);

    return 0;
}
#else
int main()
{
    int num_stalls;                   // N [2; 100 000]
    int num_cows;                     // C [2; N]
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
#endif
