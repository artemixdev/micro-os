#include "compiler.h"
#include "kernel.h"

void SVC_Handler() {}

void SysTick_Handler() {
    kernel->time += OS_TICK_INTERVAL;
}
