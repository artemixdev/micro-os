#include <stdbool.h>
#include <stdint.h>
#include "compiler.h"

NAKED bool os_create(UNUSED void (*function)(void *),
    UNUSED void *argument, UNUSED uint32_t stack) {

    asm volatile (
        "svc #2 \n"
        "bx lr \n"
    );
}

NAKED void os_switch() {
    asm volatile (
        "mov r3, sp \n"
        "push {r3} \n"
        "push {r4-r11} \n"
        "svc #0 \n"
        "pop {r4-r11} \n"
        "pop {r3} \n"
        "mov sp, r3 \n"
        "bx r0 \n"
    );
}

NAKED void os_delay(UNUSED uint32_t ms) {
    asm volatile (
        "mov r3, sp \n"
        "push {r3} \n"
        "push {r4-r11} \n"
        "svc #1 \n"
        "pop {r4-r11} \n"
        "pop {r3} \n"
        "mov sp, r3 \n"
        "bx r0 \n"
    );
}
