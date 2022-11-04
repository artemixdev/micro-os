#include <os/core.h>
#include <os/sync.h>
#include "internal.h"

__attribute__ ((always_inline))
static inline uint32_t ldrex(volatile uint32_t *address) {
    uint32_t result;
    asm volatile ("ldrex %0, %1" : "=r" (result) : "Q" (*address));
    return result;
}

__attribute__ ((always_inline))
static inline uint32_t strex(uint32_t value, volatile uint32_t *address) {
    uint32_t result;
    asm volatile ("strex %0, %2, %1" : "=&r" (result), "=Q" (*address) : "r" (value));
    return result;
}

void os_mutex_initialize(os_mutex* mutex, bool exclusive) {
    mutex->__state = 0;
    mutex->__exclusive = exclusive;
}

bool os_mutex_try_lock(os_mutex* mutex) {
    volatile uint32_t* address = &mutex->__state;

    while (true) {
        uint32_t state = ldrex(address);
        if (state != 0) {
            strex(state, address);
            return false;
        }

        state = __os_kernel.threads[__os_kernel.thread_index].id;
        if (strex(state, address) == 0) {
            return true;
        }
    }
}

void os_mutex_lock(os_mutex* mutex) {
    while (!os_mutex_try_lock(mutex)) {
        os_switch();
    }
}

bool os_mutex_try_unlock(os_mutex* mutex) {
    volatile uint32_t* address = &mutex->__state;

    while (true) {
        uint32_t state = ldrex(address);
        uint32_t id = __os_kernel.threads[__os_kernel.thread_index].id;
        if (state == 0 || (mutex->__exclusive && state != id)) {
            strex(state, address);
            return false;
        }

        if (strex(0, address) == 0) {
            return true;
        }
    }
}

void os_mutex_unlock(os_mutex* mutex) {
    while (!os_mutex_try_unlock(mutex)) {
        os_switch();
    }
}

void os_semaphore_initialize(os_semaphore* semaphore, uint32_t limit) {
    semaphore->__limit = limit;
    semaphore->__counter = 0;
}

bool os_semaphore_try_acquire(os_semaphore* semaphore, uint32_t delta) {
    volatile uint32_t* address = &semaphore->__counter;

    while (true) {
        uint32_t counter = ldrex(address);
        uint32_t new_counter = counter + delta;
        if (new_counter > semaphore->__limit) {
            strex(counter, address);
            return false;
        }

        if (strex(new_counter, address) == 0) {
            return true;
        }
    }
}

void os_semaphore_acquire(os_semaphore* semaphore, uint32_t delta) {
    while (!os_semaphore_try_acquire(semaphore, delta)) {
        os_switch();
    }
}

bool os_semaphore_try_release(os_semaphore* semaphore, uint32_t delta) {
    volatile uint32_t* address = &semaphore->__counter;

    while (true) {
        uint32_t counter = ldrex(address);
        if (delta > counter) {
            strex(counter, address);
            return false;
        }

        if (strex(counter - delta, address) == 0) {
            return true;
        }
    }
}

void os_semaphore_release(os_semaphore* semaphore, uint32_t delta) {
    while (!os_semaphore_try_release(semaphore, delta)) {
        os_switch();
    }
}
