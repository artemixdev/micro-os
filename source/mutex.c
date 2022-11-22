#include <CMSIS/ARMCM3.h>
#include <os/core.h>
#include <os/mutex.h>

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

void os_mutex_unlock(os_mutex_t *mutex) {
    __DMB();
    mutex->__state = 0;
}
