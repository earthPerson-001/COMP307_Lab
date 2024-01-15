#pragma once

void bounded_buffer_init(int buffer_size);

void bounded_buffer_deinit();

void *producer(void *params);

void *consumer(void *params);
