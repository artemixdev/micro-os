#include <CMSIS/ARMCM3.h>
#include <os/core.h>
#include "kernel.h"

void os_init(os_config_t *config) {
    kernel->running = false;
    kernel->time = 0;
    kernel->id_counter = 0;
    kernel->thread.index = -1;
    kernel->thread.value = (void *)0;
    kernel->threads.length = 0;

    SysTick_Config(config->cpu_mhz * 1000 * OS_TICK_INTERVAL);
}

void os_run() {}
