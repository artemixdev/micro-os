/**
 * @brief Static configuration used at compile time.
*/

#pragma once

#ifndef OS_THREADS_MAX
#define OS_THREADS_MAX 16
#endif

// must be a multiple of 8
#ifndef OS_MAIN_STACK_SIZE
#define OS_MAIN_STACK_SIZE 1024
#endif

// must be a multiple of 8
#ifndef OS_THREAD_STACK_SIZE_MIN
#define OS_THREAD_STACK_SIZE_MIN 128
#endif

// milliseconds
#ifndef OS_TICK_INTERVAL
#define OS_TICK_INTERVAL 1
#endif
