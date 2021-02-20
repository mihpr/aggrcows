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

uint32_t solve(int num_stalls, int num_cows, uint32_t *p_stall)
{
    uint32_t best_known_place_distance;
    int i, j;
    int cows_left = num_cows; // a counter for cows not placed yet into the stalls

    // Stalls might be unsorted, sort them
    qsort(&p_stall[0], num_stalls, sizeof(p_stall[0]), cmp_stalls);

    // There are at least 2 cows and they are placed at the first and the last stalls
    p_stall[0] |= COW_PLACED_BIT_MASK;
    p_stall[num_stalls-1] |= COW_PLACED_BIT_MASK;
    cows_left -= 2;

    // While there are still cows to be placed,
    // loop through all stalls and find the one, which is farthermost from other cows.
    // Place the a new cow there.
    // A cow will be not placed, when no cows are left. The last iteration is for calculating the minimal distance.
    while (cows_left >= 0)
    {
        uint32_t left_distance, right_distance, min_distance; // metrics of current stall
        best_known_place_distance = 0; // distance to the nearest cow
        int best_known_place_idx = -1; // index in array of stalls
        for (i = 0; i < num_stalls; i++)
        {
            // If a cow is placed here, it is impossible to place another one here, skip this stall
            if (p_stall[i] & COW_PLACED_BIT_MASK)
            {
                if (cows_left > 0)
                {
                    continue;
                }

            }

            // Calculate distance from this stall to the nearest cow to the left (in the direction with lower index).
            left_distance = 0;
            if (i > 0) // if not the leftmost position
            {
                for (j = i-1; j >= 0; j--) // search for a placed cow
                {
                    if (p_stall[j] & COW_PLACED_BIT_MASK)
                    {
                        left_distance = (p_stall[i] & COW_POSITION_MASK) - (p_stall[j] & COW_POSITION_MASK);
                        break;
                    }
                }
            }

            // Calculate distance from this stall to the nearest cow to the right (in the direction with higher index).
            right_distance = 0;
            if (i < (num_stalls-1)) // if not the rightmost position
            {
                for (j = i+1; j < num_stalls; j++) // search for a placed cow
                {
                    if (p_stall[j] & COW_PLACED_BIT_MASK)
                    {
                        right_distance = (p_stall[j] & COW_POSITION_MASK) - (p_stall[i] & COW_POSITION_MASK);
                        break;
                    }
                }

            }

            // Find distance to the nearest cow to the left or to the right
            if (right_distance < left_distance)
            {
                min_distance = right_distance;
            }
            else
            {
                min_distance = left_distance;
            }

            // If the new found position is better than the current best known position, then update it
            if (min_distance > best_known_place_distance)
            {
                best_known_place_distance = min_distance;
                best_known_place_idx = i;
            }

            // printf("%i, left_distance %u, right_distance %u, min_distance: %u\n", i, left_distance, right_distance, min_distance);
            // printf("best_known_place_distance %u, best_known_place_idx: %i\n\n", best_known_place_distance, best_known_place_idx);
        }

        // Place the cow
        if (cows_left > 0)
        {
            p_stall[best_known_place_idx] |= COW_PLACED_BIT_MASK;
        }

//        printf("Result, cows left: %i\n", cows_left);
//        for (i = 0; i < num_stalls; i++)
//        {
//            printf("[%d] 0x%08X | %u\n", i, aStall[i], aStall[i] & COW_POSITION_MASK);
//
//        }

        cows_left--;
    }

    return best_known_place_distance;
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
