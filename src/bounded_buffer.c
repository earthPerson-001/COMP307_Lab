#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "bounded_buffer.h"

int BUFFER_SIZE;
int count;
int *buffer;

int in;
int out;

// mutexes
pthread_mutex_t buffer_mutex;
pthread_mutex_t count_mutex;

// conditional variables
pthread_cond_t buffer_not_empty;
pthread_cond_t buffer_not_full;

void bounded_buffer_init(int buffer_size)
{
    BUFFER_SIZE = buffer_size;

    srand(0);
    buffer = malloc(BUFFER_SIZE * sizeof(int));

    count = 0;
    in = 0;
    out = 0;

    // mutex
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_mutex_init(&count_mutex, NULL);

    // conditional varialbes
    pthread_cond_init(&buffer_not_empty, NULL);
    pthread_cond_init(&buffer_not_full, NULL);

}

void bounded_buffer_deinit()
{
    free(buffer);

    pthread_mutex_destroy(&buffer_mutex);
    pthread_mutex_destroy(&count_mutex);
}

void *producer(void *params)
{
    int next_produced = 0;
    while (1)
    {
        next_produced = rand() % 200;
        printf("Produced Item: %d\n", next_produced);

        pthread_mutex_lock(&count_mutex);
        /* Produce an item and put in nextProduced*/
        while (count == BUFFER_SIZE)
        {
            printf("Producer waiting for buffer non-full.\n");
            pthread_cond_wait(&buffer_not_full, &count_mutex);
        }
        pthread_mutex_unlock(&count_mutex);

        pthread_mutex_lock(&buffer_mutex);
        buffer[in] = next_produced;
        pthread_mutex_unlock(&buffer_mutex);

        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_lock(&count_mutex);
        count++;
        pthread_cond_signal(&buffer_not_empty);
        pthread_mutex_unlock(&count_mutex);
    }
}

void *consumer(void *params)
{
    int next_consumed;
    while (1)
    {
        pthread_mutex_lock(&count_mutex);
        while (count == 0)
        {
            printf("Consumer waiting for buffer non-empty.\n");
            pthread_cond_wait(&buffer_not_empty, &count_mutex);
        }
        pthread_mutex_unlock(&count_mutex);

        pthread_mutex_lock(&buffer_mutex);
        next_consumed = buffer[out];
        pthread_mutex_unlock(&buffer_mutex);
        printf("Consumed Item: %d\n", next_consumed);

        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_lock(&count_mutex);
        count--;
        pthread_cond_signal(&buffer_not_full);
        pthread_mutex_unlock(&count_mutex);
    }
}
