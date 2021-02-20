#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


// Global defines
#define INPUT_FILE "./input.txt"
#define MAX_NUM_STALLS (100000)

#define COW_PLACED_BIT_MASK  (1 << 31)
#define COW_POSITION_MASK    (~COW_PLACED_BIT_MASK)


// Global settings
int num_testcases;
int num_stalls;                  // N [2; 100 000]
int num_cows;                    // C [2; N]
uint32_t aStall[MAX_NUM_STALLS]; // Stall positions [x1; xN], where xi [0; 1 000 000 000]


void init_buffers()
{
    memset(&aStall[0], 0, sizeof(aStall));

}

#define STR_BUF_SIZE (255)
// This function reads data from input file and creates test cases.
// Currently it can handle only one test case.
// TODO: Implement of handling more than one test case.
// TODO: Proper error handling.
void read_input_data()
{
    FILE *fp;
    char buff[STR_BUF_SIZE];

    fp = fopen(INPUT_FILE, "r");

    // 1-st line: number of test cases
    if (fgets(buff, STR_BUF_SIZE, fp) != NULL)
    {
       num_testcases = atoi(buff);
    }

    if (fgets(buff, STR_BUF_SIZE, fp) != NULL)
    {
        char *pToken;
        char delimiters[] = " ,.-";
        // printf ("Splitting string \"%s\" into tokens:\n",str);
        pToken = strtok(buff, delimiters);
        if (pToken != NULL) // num stalls, N
        {
            num_stalls = atoi(pToken);
            pToken = strtok(NULL, delimiters);
        }
        if (pToken != NULL) // num cows, C
        {
            num_cows = atoi(pToken);
        }
    }

    // Read stall positions
    int i, stall_position;
    for (i = 0; i < num_stalls; i++)
    {
        if (fgets(buff, STR_BUF_SIZE, fp) != NULL)
        {
            stall_position = atoi(buff);
            aStall[i] = stall_position;
        }
    }

    fclose(fp);
}


void bubble_sort_uint32(uint32_t *arr, int n)
{
   int i, j;
   uint32_t temp;
   for (i = 0; i < n-1; i++)
   {
       // Last i elements are already in place
       for (j = 0; j < n-i-1; j++)
       {
           if (arr[j] > arr[j+1])
           {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
           }
       }
   }
}

uint32_t solve()
{
    uint32_t best_known_place_distance;
    int i, j;
    int cows_left = num_cows; // a counter for cows not placed yet into the stalls

    // Stalls are unsorted, they should be sorted for convenience
    bubble_sort_uint32(aStall, num_stalls);

    // There are at least 2 cows and they are placed at the first and the last stalls
    aStall[0] |= COW_PLACED_BIT_MASK;
    aStall[num_stalls-1] |= COW_PLACED_BIT_MASK;
    cows_left -= 2;

    // While there are still cows to be placed,
    // loop through all stalls and find the one, which is farthermost from other cows.
    // Place the a new cow there.
    // A cow will be not placed, when no cows are left. The last iteration is for calculating the minimal distance.
    while (cows_left >= 0)
    {
        uint32_t left_distance, right_distance, min_distance; // metrics of current stall
        best_known_place_distance = 0; // distance to the nearest cow
        int best_known_place_idx = -1; // index in array aStall
        for (i = 0; i < num_stalls; i++)
        {
            // If a cow is placed here, it is impossible to place another one here, skip this stall
            if (aStall[i] & COW_PLACED_BIT_MASK)
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
                    if (aStall[j] & COW_PLACED_BIT_MASK)
                    {
                        left_distance = (aStall[i] & COW_POSITION_MASK) - (aStall[j] & COW_POSITION_MASK);
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
                    if (aStall[j] & COW_PLACED_BIT_MASK)
                    {
                        right_distance = (aStall[j] & COW_POSITION_MASK) - (aStall[i] & COW_POSITION_MASK);
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
            aStall[best_known_place_idx] |= COW_PLACED_BIT_MASK;
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
    init_buffers();
    read_input_data();
    printf("Test cases: %i, cows: %i, stalls: %i\n", num_testcases, num_cows, num_stalls);

    uint32_t result = solve();
    printf("Output: %u", result);


    return 0;
}
