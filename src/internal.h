#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <os/config.h>

typedef struct {
    uint32_t id;
    uint32_t left_at, sleep;
    uint32_t argument_address;
    uint32_t registers[8]; // r4-r11
    uint32_t text_address, text_offset;
    uint32_t stack_area, stack_size;
} os_thread;

typedef struct {
    os_thread threads[OS_THREADS_MAX];
    uint32_t threads_length, thread_index;
    bool areas[OS_THREADS_MAX];
    uint32_t id_counter;
    bool running;
    volatile uint32_t time;
} os_kernel;

extern os_kernel __os_kernel;

void __os_interceptor();
void __os_epilogue();
void __os_schedule();
