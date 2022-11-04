#include "internal.h"

void os_initialize(uint32_t (*SysTick_Config)(uint32_t ticks)) {
    __os_kernel.threads_length = 0;
    __os_kernel.thread_index = 0;
    __os_kernel.id_counter = 0;
    __os_kernel.running = false;

    for (uint32_t index = 0; index < OS_THREADS_MAX; index++) {
        __os_kernel.areas[index] = false;
    }

    SysTick_Config(OS_CPU_FREQUENCY / 1000 * OS_SYSTICK_INTERVAL);
}

__attribute__ ((naked))
void os_run() {
    asm volatile ("b os_switch");
}

bool os_create(void (*task)(void*), void* argument) {
    uint32_t new_length = __os_kernel.threads_length + 1;
    if (new_length > OS_THREADS_MAX) {
        return false;
    }

    uint32_t area = 0;
    for (; area < OS_THREADS_MAX; area++) {
        if (!__os_kernel.areas[area]) {
            break;
        }
    }

    os_thread* thread = &__os_kernel.threads[new_length - 1];
    thread->id = ++__os_kernel.id_counter;
    thread->left_at = 0;
    thread->sleep = 0;
    thread->argument_address = (uint32_t)argument;
    thread->text_address = (uint32_t)task;
    thread->text_offset = 0;
    thread->stack_area = area;
    thread->stack_size = 0;

    __os_kernel.areas[area] = true;
    __os_kernel.threads_length = new_length;

    return true;
}

__attribute__ ((naked))
void os_sleep(uint32_t milliseconds) {
    asm volatile (
        "mov r0, %[ms]\n"
        "mov r1, sp \n"
        "mov sp, %[address] \n"
        "mov r2, #0 \n"
        "push {r0} \n" // thread sleep milliseconds, input
        "push {r2} \n" // thread completed flag, input
        "push {r2} \n" // next thread argument address stub, output
        "push {lr} \n" // thread return address, input-output
        "push {r1} \n" // thread stack pointer, input-output
        "push {r4-r11} \n" // thread registers, input-output
        "bl __os_schedule \n"
        "b __os_epilogue \n"
        :: [ms] "r" (milliseconds), [address] "r" (OS_MAIN_STACK_ADDRESS)
    );
}

__attribute__ ((naked))
void os_switch() {
    asm volatile (
        "mov r1, sp \n"
        "mov sp, %[address] \n"
        "mov r2, #0 \n"
        "push {r2} \n" // thread sleep milliseconds, input
        "push {r2} \n" // thread completed flag, input
        "push {r2} \n" // next thread argument address stub, output
        "push {lr} \n" // thread return address, input-output
        "push {r1} \n" // thread stack pointer, input-output
        "push {r4-r11} \n" // thread registers, input-output
        "bl __os_schedule \n"
        "b __os_epilogue \n"
        :: [address] "r" (OS_MAIN_STACK_ADDRESS)
    );
}
