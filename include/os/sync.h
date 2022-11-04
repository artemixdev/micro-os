#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t __state;
    bool __exclusive;
} os_mutex;

void os_mutex_initialize(os_mutex*, bool exclusive);
bool os_mutex_try_lock(os_mutex*);
void os_mutex_lock(os_mutex*);
bool os_mutex_try_unlock(os_mutex*);
void os_mutex_unlock(os_mutex*);

typedef struct {
    uint32_t __limit;
    uint32_t __counter;
} os_semaphore;

void os_semaphore_initialize(os_semaphore*, uint32_t limit);
bool os_semaphore_try_acquire(os_semaphore*, uint32_t delta);
void os_semaphore_acquire(os_semaphore*, uint32_t delta);
bool os_semaphore_try_release(os_semaphore*, uint32_t delta);
void os_semaphore_release(os_semaphore*, uint32_t delta);
