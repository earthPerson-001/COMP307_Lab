#pragma once

void matrix_input(int *M, int row, int col);

void vector_input(int *M, int col);

void calculate_need(int *max, int *allocation, int *need, int row, int col);

void print_matrix(int *M, int row, int col);

void print_vector(int *M, int col);

void print_all(int *allocation, int* max, int * need, int row, int col);


/**
 * Returns 1 if the allocation leaves the system in safe state
 * else returns 0
 *
 * The sequence will be filled with process numbers which produces a safe state(if any)
 * 
 */
int run_safety_algorithm(int *need, int *allocation, int *available, int row, int col, int *sequence);
