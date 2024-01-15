#pragma once

typedef struct process_t {
    int pid;
    int burst_time;
    int arrival_time;
    int waiting_time;
    int turn_around_time;
    int completion_time;
    int service_time;
} process_t;

void initialize_process(process_t** process, int pid, int burst_time, int arrival_time, int waiting_time, int turn_around_time, int completion_time, int service_time);

void display_process(int pid, int arrival_time, int burst_time, int completion_time, int turn_around_time, int waiting_time);

void display_processes(process_t** processes, int len);

void fcfs_zero_arrival_time(process_t** processes, int len);

void fcfs(process_t **processes, int len);

void sjf(process_t **processes, int len);

void rr(process_t **processes, int len, int quantum_size);

void destroy_processes(process_t** process, int len);

void destroy_process(process_t* process);
