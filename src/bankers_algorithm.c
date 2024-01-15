#include <stdio.h>
#include <stdlib.h>
#include "bankers_algorithm.h"

void matrix_input(int *M, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("Enter for process %d and resource %d: ", i, j);
            scanf("%d", &M[i * col + j]);
        }
    }
}

void vector_input(int *M, int col)
{
    for (int j = 0; j < col; j++)
    {
        printf("Resource %d: ", j);
        scanf("%d", &M[j]);
    }
}

void print_matrix(int *M, int row, int col)
{

    for (int i = 0; i < row; i++)
    {
        printf("Process:%d Resources: ", i);
        for (int j = 0; j < col; j++)
        {
            printf("%d ", M[i * col + j]);
        }
        printf("\n");
    }
}

void print_all(int *allocation, int *max, int *need, int row, int col)
{

    printf("Process\t\tAllocation\tMax\tNeed\n");
    for (int i = 0; i < row; i++)
    {
        printf("Process %d:\t", i);
        for (int j = 0; j < col; j++)
        {
            printf("%d ", allocation[i * col + j]);
        }

        printf("\t\t");
        for (int j = 0; j < col; j++)
        {
            printf("%d ", max[i * col + j]);
        }

        printf("\t");
        for (int j = 0; j < col; j++)
        {
            printf("%d ", need[i * col + j]);
        }

        printf("\n");
    }
    printf("\n");
}

void print_vector(int *M, int col)
{
    for (int j = 0; j < col; j++)
    {
        printf("%d ", M[j]);
    }
}

/**
 * @param need: output
 */
void calculate_need(int *max, int *allocation, int *need, int row, int col)
{

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            need[i * col + j] = max[i * col + j] - allocation[i * col + j];
        }
    }
}

static int check_need_less_than_work(int *need, int *work, int row, int col)
{
    for (int i = 0; i < col; i++)
    {
        if (need[row * col + i] > work[i])
        {
            return 0;
        }
    }
    return 1;
}

static void add_allocation_to_work(int *allocation, int *work, int row, int col)
{
    for (int i = 0; i < col; i++)
    {
        work[i] = work[i] + allocation[row * col + i];
    }
}

/**
 * Returns 1 if the allocation leaves the system in safe state
 * else returns 0
 *
 * The sequence will be filled with process numbers which produces a safe state(if any)
 *
 */
int run_safety_algorithm(int *need, int *allocation, int *available, int row, int col, int *sequence)
{

    int *work = malloc(col * sizeof(int));
    int *finished = malloc(row * sizeof(int));

    // copying available to work
    for (int i = 0; i < col; i++)
    {
        work[i] = available[i];
    }

    // none of the process has been finished initially
    for (int j = 0; j < row; j++)
    {
        finished[j] = 0;
    }

    int found_executable_process = 0;
    int finished_count = 0;
    int k = 0;
    int continue_looping = 1;
    int ret_val = 0;
    int checked_from_first = 1;
    while (continue_looping)
    {
        found_executable_process = 0;
        checked_from_first = 0;
        // continue from previous index
        if (k >= row) {
            k = 0;
            checked_from_first = 1;
        }

        for (; k < row; k++)
        {
            if ((finished[k] == 0) && check_need_less_than_work(need, work, k, col))
            {
                found_executable_process = 1;
                break;
            }
        }

        // check from first if none of the processes requirement can be met
        if(!found_executable_process && !checked_from_first) {
            k += 1;
            continue;
        }

        if (found_executable_process == 1)
        {
            add_allocation_to_work(allocation, work, k, col);
            finished[k] = 1;
            sequence[finished_count] = k;
            finished_count += 1;
            printf("Process P%d's Resource Requirement can be met, new available: ", k);
            for(int j =0; j < col; j++) {
                printf("%d ", work[j]);
            }
            printf("\n");
        }
        else
        {

            ret_val = 1; // the return value will be reset if any of the process isn't finished
            for (int l = 0; l < row; l++)
            {

                if (finished[l] == 0)
                {
                   ret_val = 0;
                }
            }
            continue_looping = 0;
        }

        // skip the previous index
        k += 1;
    }

    free(work);
    free(finished);

    return ret_val;
}
