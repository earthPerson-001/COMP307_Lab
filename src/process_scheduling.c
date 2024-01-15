#include "process_scheduling.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_process(process_t **process, int pid, int burst_time, int arrival_time, int waiting_time, int turn_around_time, int completion_time, int service_time)
{
    *process = malloc(sizeof(process_t));

    (*process)->pid = pid;
    (*process)->burst_time = burst_time;
    (*process)->arrival_time = arrival_time;
    (*process)->waiting_time = waiting_time;
    (*process)->turn_around_time = turn_around_time;
    (*process)->completion_time = completion_time;
    (*process)->service_time = service_time;
}

void display_process(int pid, int arrival_time, int burst_time, int completion_time, int turn_around_time, int waiting_time)
{
    printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", pid, arrival_time, burst_time, completion_time, turn_around_time, waiting_time);
}

void display_processes(process_t **processes, int len)
{
    // displaying the processes
    printf("PID\tArrival Time\tBurst Time\tCompletion Time\tTurn Around Time\tWaiting Time\n");
    for (int i = 0; i < len; i++)
    {

        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", processes[i]->pid, processes[i]->arrival_time, processes[i]->burst_time, processes[i]->completion_time, processes[i]->turn_around_time, processes[i]->waiting_time);
    }
}

void destroy_processes(process_t **process, int len)
{
    for (int i = 0; i < len; i++)
    {
        free(process[i]);
    }
}

void destroy_process(process_t *process)
{
    free(process);
}

void fcfs_zero_arrival_time(process_t **processes, int len)
{
    // assume all the arrival times are zero
    for (int i = 0; i < len; i++)
    {
        processes[i]->arrival_time = 0;
    }

    processes[0]->completion_time = processes[0]->burst_time - processes[0]->arrival_time;
    processes[0]->waiting_time = 0;
    for (int i = 1; i < len; i++)
    {
        processes[i]->completion_time = processes[i - 1]->completion_time + processes[i]->burst_time - processes[i]->arrival_time;
        processes[i]->turn_around_time = processes[i]->completion_time - processes[i]->arrival_time;
        processes[i]->waiting_time = processes[i]->completion_time - processes[i]->arrival_time - processes[i]->burst_time;
    }
}

static void swap(process_t **processes, int i, int j)
{
    process_t *temp = processes[i];
    processes[i] = processes[j];
    processes[j] = temp;
}

void fcfs(process_t **processes, int len)
{
    // sorting based on arrival time
    int i, j;
    int swapped;
    for (i = 0; i < len; i++)
    {
        swapped = 0;
        for (j = 0; j < len - 1; j++)
        {
            if (processes[j]->arrival_time > processes[j + 1]->arrival_time)
            {
                swap(processes, j, j + 1);
                swapped = 1;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == 0)
            break;
    }

    // we should have a sorted processes based on arrival_time
    processes[0]->completion_time = processes[0]->burst_time + processes[0]->arrival_time;
    processes[0]->waiting_time = 0;
    processes[0]->turn_around_time = processes[0]->completion_time - processes[0]->arrival_time;
    int current_time = processes[0]->burst_time + processes[0]->arrival_time;
    for (int i = 1; i < len; i++)
    {

        // only increase time if it hasn't arrived yet
        if (processes[i]->arrival_time > current_time)
        {
            current_time += 1;
            i -= 1;
            continue;
        }

        processes[i]->completion_time = current_time + processes[i]->burst_time;
        processes[i]->turn_around_time = processes[i]->completion_time - processes[i]->arrival_time;
        processes[i]->waiting_time = processes[i]->completion_time - processes[i]->arrival_time - processes[i]->burst_time;

        current_time += processes[i]->burst_time;
    }
}

int find_pid(int *pids, int pid, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (pids[i] == pid)
        {
            return 1;
        }
    }
    return 0;
}

void sjf(process_t **processes, int len)
{
    // sorting based on burst time
    int i, j;
    int swapped;
    for (i = 0; i < len; i++)
    {
        swapped = 0;
        for (j = 0; j < len - 1; j++)
        {
            if (processes[j]->burst_time > processes[j + 1]->burst_time)
            {
                swap(processes, j, j + 1);
                swapped = 1;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == 0)
            break;
    }

    // declaring loop to run the processes
    int current_time = 0;
    int *completed_pids = malloc(len * sizeof(int));
    int completed_pids_count = 0;

    while (completed_pids_count < len)
    {
        int prev_completed_pids_count = completed_pids_count;
        for (int i = 0; i < len; i++)
        {
            if (1 == find_pid(completed_pids, processes[i]->pid, completed_pids_count))
            {
                continue;
            }
            if (processes[i]->arrival_time <= current_time)
            {
                processes[i]->completion_time = current_time + processes[i]->burst_time;
                processes[i]->turn_around_time = processes[i]->completion_time - processes[i]->arrival_time;
                processes[i]->waiting_time = processes[i]->completion_time - processes[i]->arrival_time - processes[i]->burst_time;
                current_time += processes[i]->burst_time;
                completed_pids[completed_pids_count] = processes[i]->pid;
                completed_pids_count += 1;
                processes[i]->service_time += processes[i]->burst_time;
                // again searching from the first
                // assuming the for syntax will increment it by one
                i = -1;
            }
        }

        // increasing current_time, if none of the process have arrived yet
        if (prev_completed_pids_count == completed_pids_count)
        {
            current_time += 1;
        }
    }

    free(completed_pids);
}

void rr(process_t **processes, int len, int quantum_size)
{

    // sorting based on arrival time
    int i, j;
    int swapped;
    for (i = 0; i < len; i++)
    {
        swapped = 0;
        for (j = 0; j < len - 1; j++)
        {
            if (processes[j]->arrival_time > processes[j + 1]->arrival_time)
            {
                swap(processes, j, j + 1);
                swapped = 1;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == 0)
            break;
    }

    // resetting the service time
    for (int k = 0; k < len; k++)
    {
        processes[k]->service_time = 0;
    }

    int current_time = 0;
    int remaining_time = 0;
    int time_elapsed = 0;
    int n_completed = 0;
    while (n_completed < len)
    {
        for (int i = 0; i < len; i++)
        {
            // continue if the process hasn't arrived yet
            if (processes[i]->arrival_time > current_time)
            {
                current_time += 1;
                i -= 1;
                continue;
            }

            // skip if it has already got the required burst time
            if (processes[i]->service_time >= processes[i]->burst_time)
            {
                continue;
            }

            remaining_time = processes[i]->burst_time - processes[i]->service_time;
            time_elapsed = (remaining_time < quantum_size) ? remaining_time : quantum_size;

            // execute the process for a period of quantum size
            processes[i]->service_time += time_elapsed;

            if (processes[i]->service_time >= processes[i]->burst_time)
            { // the process has just completed
                processes[i]->completion_time = current_time + time_elapsed;
                processes[i]->turn_around_time = processes[i]->completion_time - processes[i]->arrival_time;
                processes[i]->waiting_time = processes[i]->completion_time - processes[i]->arrival_time - processes[i]->burst_time;
                n_completed += 1;
            }

            current_time += time_elapsed;
        }
    }
}