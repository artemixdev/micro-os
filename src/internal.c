#include "internal.h"

os_kernel __os_kernel;

__attribute__ ((naked))
void __os_interceptor() {
    asm volatile (
        "mov r1, sp \n"
        "mov sp, %[address] \n"
        "mov r2, #1 \n"
        "mov r3, #0 \n"
        "push {r3} \n" // thread sleep milliseconds, input
        "push {r2} \n" // thread completed flag, input
        "push {r3} \n" // next thread argument address stub, output
        "push {lr} \n" // thread return address, input-output
        "push {r1} \n" // thread stack pointer, input-output
        "push {r4-r11} \n" // thread registers, input-output
        "bl __os_schedule \n"
        "b __os_epilogue \n"
        :: [address] "r" (OS_MAIN_STACK_ADDRESS)
    );
}

__attribute__ ((naked))
void __os_epilogue() {
    asm volatile (
        "pop {r4-r11} \n" // next thread registers
        "pop {r1} \n" // next thread stack pointer
        "pop {r2} \n" // next thread return address
        "pop {r0} \n" // next thread argument address
        "mov sp, r1 \n"
        "mov lr, %[address] \n"
        "bx r2 \n"
        :: [address] "r" (&__os_interceptor)
    );
}

// todo: __attribute__ ((packed)) ?
typedef struct {
    uint32_t registers[8]; // r4-r11
    uint32_t stack_pointer;
    uint32_t return_address;
    uint32_t argument_address;
    uint32_t completed;
    uint32_t sleep_milliseconds;
} os_context;

static uint32_t to_stack_address(uint32_t stack_area) {
    return stack_area * -1 * OS_THREAD_STACK_SIZE
        + OS_MAIN_STACK_ADDRESS - OS_MAIN_STACK_SIZE;
}

static void handle_current_thread(os_context* context) {
    os_thread* thread = &__os_kernel.threads[__os_kernel.thread_index];
    bool edge = __os_kernel.thread_index == __os_kernel.threads_length - 1;

    if (context->completed && edge) {
        __os_kernel.thread_index = 0;
    }

    if (context->completed && !edge) {
        uint32_t index = __os_kernel.thread_index;
        for (; index < __os_kernel.threads_length - 1; index++) {
            __os_kernel.threads[index] = __os_kernel.threads[index + 1];
        }
    }

    if (context->completed) {
        __os_kernel.areas[thread->stack_area] = false;
        __os_kernel.threads_length--;
        return;
    }

    for (uint32_t i = 0; i < 8; i++) {
        thread->registers[i] = context->registers[i];
    }
    thread->text_offset = (context->return_address) - thread->text_address;
    thread->stack_size = to_stack_address(thread->stack_area) - (context->stack_pointer);
    thread->left_at = __os_kernel.time;
    thread->sleep = context->sleep_milliseconds;

    __os_kernel.thread_index = (__os_kernel.thread_index + 1) % __os_kernel.threads_length;
}

static void handle_thread_readiness() {
    while (true) {
        os_thread* thread = &__os_kernel.threads[__os_kernel.thread_index];
        bool ready = __os_kernel.time >= thread->left_at + thread->sleep;
        if (ready) {
            return;
        }

        __os_kernel.thread_index = (__os_kernel.thread_index + 1) % __os_kernel.threads_length;
    }
}

static void handle_next_thread(os_context* context) {
    os_thread* thread = &__os_kernel.threads[__os_kernel.thread_index];
    for (uint32_t i = 0; i < 8; i++) {
        context->registers[i] = thread->registers[i];
    }
    context->return_address = thread->text_address + thread->text_offset;
    context->stack_pointer = to_stack_address(thread->stack_area) - thread->stack_size;
    context->argument_address = thread->argument_address;
}

void __os_schedule() {
    os_context* context = (os_context*)(OS_MAIN_STACK_ADDRESS - sizeof(os_context));

    if (__os_kernel.running) {
        handle_current_thread(context);
    }

    if (__os_kernel.threads_length == 0) {
        while (true); // todo: wfi?
    }

    handle_thread_readiness();
    handle_next_thread(context);

    __os_kernel.running = true;
}

void SysTick_Handler() {
    __os_kernel.time += OS_SYSTICK_INTERVAL;
}
