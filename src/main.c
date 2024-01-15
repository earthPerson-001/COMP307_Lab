#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "process_scheduling.h"
#include "bankers_algorithm.h"
#include "bounded_buffer.h"

#define N_PROCESSES 3
#define QUANTUM_SIZE 3

#define BOUNDED_BUFFER_SIZE 5

void process_scheduling()
{
    // intialize place for process
    process_t *processes[N_PROCESSES];
    int pid, burst_time, arrival_time;

    // take input from the user
    for (int i = 0; i < N_PROCESSES; i++)
    {
        printf("Enter PID for process %d: ", i);
        scanf("%d", &pid);
        printf("Enter Burst Time for process %d: ", i);
        scanf("%d", &burst_time);
        printf("Enter Arrival Time for process %d: ", i);
        scanf("%d", &arrival_time);

        initialize_process(&processes[i], pid, burst_time, arrival_time, 0, 0, 0, 0);
    }

    printf("Running FCFS scheduling algorithm.\n");
    // assume all the arrival times are zero
    fcfs(processes, N_PROCESSES);

    // displaying the processes
    display_processes(processes, N_PROCESSES);

    printf("Running SJF scheduling algorithm.\n");

    sjf(processes, N_PROCESSES);

    display_processes(processes, N_PROCESSES);

    printf("Running RR scheduling algorithm, with quantum size %d\n", QUANTUM_SIZE);

    rr(processes, N_PROCESSES, QUANTUM_SIZE);

    display_processes(processes, N_PROCESSES);

    destroy_processes(processes, N_PROCESSES);
}

void bankers_algorithm()
{
    int n_processes = 0;
    int n_resources = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n_processes);
    printf("Enter the number of resources: ");
    scanf("%d", &n_resources);

    int *max = malloc((n_processes * n_resources) * sizeof(int));
    int *allocation = malloc((n_processes * n_resources) * sizeof(int));
    int *need = malloc((n_processes * n_resources) * sizeof(int));

    int *available = malloc(n_processes * sizeof(int));

    printf("Enter the entries for max resource requirement: \n");
    matrix_input(max, n_processes, n_resources);
    printf("\n-------------------------------------------------------\n");
    printf("Entries for max resource requirement \n");
    print_matrix(max, n_processes, n_resources);
    printf("\n-------------------------------------------------------\n");
    printf("Enter the entries for current allocation: \n");
    matrix_input(allocation, n_processes, n_resources);
    printf("\n-------------------------------------------------------\n");
    printf("Entries for current allocation \n");
    print_matrix(allocation, n_processes, n_resources);
    printf("\n-------------------------------------------------------\n");

    printf("Enter the entries for available: \n");
    vector_input(available, n_resources);
    printf("\n-------------------------------------------------------\n");

    printf("Entries for available: ");
    print_vector(available, n_resources);
    printf("\n-------------------------------------------------------\n");
    printf("\n");

    calculate_need(max, allocation, need, n_processes, n_resources);
    print_all(allocation, max, need, n_processes, n_resources);

    int *safe_sequence = malloc(n_processes * sizeof(int));
    if (1 == run_safety_algorithm(need, allocation, available, n_processes, n_resources, safe_sequence))
    {
        printf("\nThe allocations: <");
        for (int i = 0; i < n_processes; i++)
        {
            printf("P%d,", safe_sequence[i]);
        }
        printf(" > results in safe state.\n");
    }
    else
    {
        printf("The allocations results in unsafe state.\n");
    }

    free(max);
    free(allocation);
    free(need);
    free(available);
    free(safe_sequence);
}

void bounded_buffer()
{
    pthread_t producer_thread, consumer_thread;
    int i_ret_producer, i_ret_consumer;

    // initialize
    bounded_buffer_init(BOUNDED_BUFFER_SIZE);

    /* Create independent threads for producer and consumer*/
    i_ret_producer = pthread_create(&producer_thread, NULL, producer, (void *)NULL);
    i_ret_consumer = pthread_create(&consumer_thread, NULL, consumer, (void *)NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    bounded_buffer_deinit();
}

int main()
{
    // process_scheduling();

    // bankers_algorithm();

    bounded_buffer();

    return 0;
}
