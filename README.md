# MicroOS
![](https://github.com/artemixdev/micro-os/actions/workflows/build.yml/badge.svg?branch=main)

MicroOS is a micro operating system designed for use in microcontrollers.
Its main purpose is to be minimalistic, transparent, and easy to use.
The system provides basic non-preemptive multitasking
allowing you to execute your tasks sequentially.
It also provides a simple API for interacting with the system at run time.
Currently, the focus is on ARM, but other popular architectures will be covered.

## Main features
- 🚀 Creating threads
- 🔀 Switching threads
- ⏰ Putting threads to sleep
- ⚙️ Synchronization primitives

## Supported architectures
- ✅ ARMv7-M and above
- ⚠️ ARMv6-M (partial support or not tested)

## Some details you need to know
The system uses `SysTick` timer for scheduling. Hence, you have to avoid using `SysTick` timer
for your purposes. Perhaps this behavior will be changed in the future
but today there is no reason for this.

## Installation
The easiest and most common way is to add contents of `include` and `src` to your project.
Note that the configuration is set at compile time by defining certain symbols.

## Example with STM32F1
```c
#include <stm32f1xx.h>
#include <os/core.h>

void blink_pb0(void* argument) {
    uint32_t sleep = *(uint32_t*)(argument);
    while (true) {
        GPIOB->BSRR &= ~(GPIO_BSRR_BR0);
        GPIOB->BSRR |= GPIO_BSRR_BS0;
        os_sleep(sleep);
        GPIOB->BSRR &= ~(GPIO_BSRR_BS0);
        GPIOB->BSRR |= GPIO_BSRR_BR0;
        os_sleep(sleep);
    }
}

void blink_pb1(void* argument) {
    uint32_t sleep = *(uint32_t*)(argument);
    while (true) {
        GPIOB->BSRR &= ~(GPIO_BSRR_BR1);
        GPIOB->BSRR |= GPIO_BSRR_BS1;
        os_sleep(sleep);
        GPIOB->BSRR &= ~(GPIO_BSRR_BS1);
        GPIOB->BSRR |= GPIO_BSRR_BR1;
        os_sleep(sleep);
    }
}

uint32_t argument1 = 250, argument2 = 333;

void main() {
    RCC->APB2ENR = RCC_APB2ENR_IOPBEN;
    GPIOB->CRL = GPIO_CRL_MODE1_1 | GPIO_CRL_MODE0_1;

    os_initialize(&SysTick_Config);
    os_create(&blink_pb0, &argument1);
    os_create(&blink_pb1, &argument2);
    os_run();
}
```
