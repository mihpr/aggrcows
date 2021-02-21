#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Description of the task
// https://www.spoj.com/problems/AGGRCOW/

#define INPUT_FILE "D:\\data\\git\\aggrcows\\input.txt"
//#define INPUT_FILE "./input.txt" // TODO: delete
#define MAX_NUM_STALLS (100000)

#define COW_PLACED_BIT_MASK  (1 << 31)
#define COW_POSITION_MASK    (~COW_PLACED_BIT_MASK)

#define STR_BUF_SIZE (256)

void init_buffers(uint32_t *p_stall, size_t buf_len)
{
    memset(&p_stall[0], 0, sizeof(uint32_t) * buf_len);
}


void dbg_print_stalls(uint32_t *p_stall, size_t stall_cnt)
{
    printf("stalls:\n");
    int i;
    for (i = 0; i < (int)stall_cnt; i++)
    {
        printf("    %i: [%i]\n", i, p_stall[i]);
    }
    printf("\n");
}

int read_num_testcases(FILE * fp)
{
    char buff[STR_BUF_SIZE];

    // Read number of test cases
    if (fgets(buff, STR_BUF_SIZE, fp) == NULL)
    {
        printf("Error: failed to read the 1-st line of the input file: number of test cases.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    return atoi(buff);
}

void read_test_case(FILE * fp, int *p_num_stalls, int *p_num_cows, uint32_t *p_stall)
{
    char buff[STR_BUF_SIZE];
    char *pToken;
    char delimiters[] = " ,.-";

    // Two space-separated integers: number of stalls and number of cows.
    if (fgets(buff, STR_BUF_SIZE, fp) == NULL)

    {
        printf("Error: failed to read the number of stalls and number of cowss.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // printf ("Splitting string \"%s\" into tokens:\n",str);
    pToken = strtok(buff, delimiters);
    if (pToken == NULL) // num stalls, N
    {
        printf("Error: failed to get the number of stalls.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    *p_num_stalls = atoi(pToken);

    pToken = strtok(NULL, delimiters);

    if (pToken == NULL) // num cows, C
    {
        printf("Error: failed to get the number of cows.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    *p_num_cows = atoi(pToken);

    // Read stall positions
    int i, stall_position;
    for (i = 0; i < *p_num_stalls; i++)
    {
        if (fgets(buff, STR_BUF_SIZE, fp) == NULL)
        {
            printf("Error: failed to read the number of stalls at position [%i].\n", i);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        stall_position = atoi(buff);
        p_stall[i] = stall_position;
    }
}

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
        printf("mid = [%u]\n", mid);
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

    FILE * fp;
    fp = fopen(INPUT_FILE, "r");

    if (fp == NULL)
    {
       printf("Error: no input file was found at the given path [%s]\n", INPUT_FILE);
       exit(EXIT_FAILURE);
    }

    int num_testcases = read_num_testcases(fp);
    printf("Number of test cases: %i\n\n", num_testcases);
    int t;
    for (t = 1; t <= num_testcases; t++)
    {
        init_buffers(&a_stall[0], MAX_NUM_STALLS);

        printf("Test case: [%i]\n", t);
        read_test_case(fp, &num_stalls, &num_cows,  &a_stall[0]);
        printf("num_stalls: [%i], num_cows: [%i]\n", num_stalls, num_cows);
//        dbg_print_stalls(&a_stall[0], num_stalls);

        uint32_t result = solve(num_stalls, num_cows, &a_stall[0]);
        printf("solve result: %u\n\n", result);
    }

    fclose(fp);
    return 0;
}
