#pragma once

#include <stdbool.h>
#include <stdint.h>

// Initializers.

typedef struct {
    uint16_t cpu_mhz;
} os_config_t;

/**
 * @brief Initializes the system.
 * @details Must be used only once and only outside a thread (usually in main).
*/
void os_init(os_config_t *config);

/**
 * @brief Runs the system.
 * @details Must be used only once and only outside a thread (usually in main).
*/
void os_run();

// System calls.

/**
 * @brief Creates a new thread.
 * @param function Thread function.
 * @param argument Thread function argument.
 * @param stack Thread stack size.
 * @return A new thread was created.
*/
bool os_create(void (*function)(void *argument), void *argument, uint32_t stack);

/**
 * @brief Switches current thread.
 * @details Must be used only inside a thread.
*/
void os_switch();

/**
 * @brief Switches current thread and puts it to sleep.
 * @details Must be used only inside a thread.
 * @param ms Minimum sleep time in milliseconds.
*/
void os_delay(uint32_t ms);
