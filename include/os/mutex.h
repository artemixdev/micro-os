#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t __state;
} os_mutex_t;

bool os_mutex_try_lock(os_mutex_t *mutex);
void os_mutex_lock(os_mutex_t *mutex);
void os_mutex_unlock(os_mutex_t *mutex);
