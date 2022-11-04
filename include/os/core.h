#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Initializes the kernel.
 * \details Must be used only once and only outside a task (usually in main).
 * \param SysTick_Config The original SysTick_Config function.
*/
void os_initialize(uint32_t (*SysTick_Config)(uint32_t ticks));

/**
 * \brief Runs the system.
 * \details Must be used only once and only outside a task (usually in main).
*/
void os_run();

/**
 * \brief Creates a new thread.
 * \details Receives a task to create a new thread executing this task.
 * \param task A function to execute.
 * \param argument A pointer to an argument passed to the task.
 * \return True if a new thread was created successfully.
*/
bool os_create(void (*task)(void* argument), void* argument);

/**
 * \brief Puts the thread into the sleep mode.
 * \details Switches the thread to another thread until the time interval has elapsed.
 * \param milliseconds The minimal amount of milliseconds to wait.
*/
void os_sleep(uint32_t milliseconds);

/**
 * \brief Tells the system to switch to another thread.
 * \details Another thread will be executed if exists.
 * Otherwise, the same thread will continue its execution.
*/
void os_switch();
