#include <CMSIS/ARMCM3.h>
#include <os/core.h>
#include <os/mutex.h>
#include "compiler.h"

bool os_mutex_try_lock(os_mutex_t *mutex) {
    volatile uint8_t *address = &mutex->__state;

    while (true) {
        if (__LDREXB(address) != 0) {
            __CLREX();
            return false;
        }

        if (__STREXB(1, address) == 0) {
            __DMB();
            return true;
        }
    }
}

void os_mutex_lock(os_mutex_t *mutex) {
    while (!os_mutex_try_lock(mutex)) {
        os_switch();
    }
}

NAKED void os_mutex_unlock(UNUSED os_mutex_t *mutex) {
    asm volatile (
        "mov r1, #0 \n"
        "dmb \n"
        "strb r1, [r0] \n"
        "bx lr \n"
    );
}
