#include "compiler.h"
#include "kernel.h"

#ifdef OS_LIBOPENCM3
void sv_call_handler()
#else
void SVC_Handler()
#endif
{}

#ifdef OS_LIBOPENCM3
void sys_tick_handler()
#else
void SysTick_Handler()
#endif
{
    kernel->time += OS_TICK_INTERVAL;
}
