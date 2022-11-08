#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <os/config.h>

typedef struct {
    uint32_t id;
    uint64_t switched_at;
    uint32_t delay;
    void *argument;
    uint32_t core_registers[8]; // r4-r11
    uint32_t sp, pc;
} thread_t;

typedef struct {
    uint32_t thread_id;
    uint32_t address;
    uint32_t size;
} stack_record_t;

typedef struct {
    bool running;
    volatile uint64_t time;
    uint32_t id_counter;
    struct {
        int32_t index;
        thread_t *value;
    } thread;
    struct {
        uint32_t length;
        thread_t values[OS_THREADS_MAX];
    } threads;
    stack_record_t stack_records[OS_THREADS_MAX]; // must be sorted by address descending
} kernel_t;

extern uint8_t _estack, _ebss;

#define kernel ((kernel_t *)((uint32_t)&_estack + 1 - sizeof(kernel_t)))
