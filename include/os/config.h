#pragma once

#ifndef OS_MAIN_STACK_ADDRESS
    #define OS_MAIN_STACK_ADDRESS 0x20005000
#endif

#ifndef OS_MAIN_STACK_SIZE
    #define OS_MAIN_STACK_SIZE 1024
#endif

#ifndef OS_THREAD_STACK_SIZE
    #define OS_THREAD_STACK_SIZE 256
#endif

#ifndef OS_THREADS_MAX
    #define OS_THREADS_MAX 10
#endif

#if defined(F_CPU) && !defined(OS_CPU_FREQUENCY)
    #define OS_CPU_FREQUENCY F_CPU
#endif

#ifndef OS_CPU_FREQUENCY
    #define OS_CPU_FREQUENCY 8000000
#endif

#ifndef OS_SYSTICK_INTERVAL
    #define OS_SYSTICK_INTERVAL 1 // milliseconds
#endif
