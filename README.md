# micro-os
![](https://github.com/artemixdev/micro-os/actions/workflows/build.yml/badge.svg?branch=main)

micro-os is a micro operating system designed for use in microcontrollers.
Its main purpose is to be minimalistic, transparent, and easy to use.
The system provides basic non-preemptive multitasking
allowing you to execute your tasks sequentially.
It also provides a simple API for interacting with the system at run time.
Currently, the focus is on ARM, but other popular architectures will be considered.

## Main features
- 🚀 Creating threads
- 🔀 Switching threads
- ⏰ Putting threads to sleep
- ⚙️ Synchronization primitives

## Supported architectures
- ✅ Cortex-M3

## Installation
The easiest and most common way is to add contents of `include` and `source` to your project.
Note that the configuration is set at compile time by defining certain symbols.

## Some details you need to know
- The system uses `SysTick` timer for scheduling. Hence, you have to avoid using `SysTick` timer
for your purposes. Perhaps this behavior will be changed in the future
but today there is no reason for this.
- It's implied you have `_estack` and `_ebss` symbols in your linker script file.
- ISRs use stack of current thread (this behavior will be changed).

## Example with STM32F1
```c
#include <stm32f1xx.h>
#include <os/core.h>

void blink_pb0(void* argument) {
    uint32_t delay = *(uint32_t *)argument;
    while (true) {
        GPIOB->BSRR &= ~(GPIO_BSRR_BR0);
        GPIOB->BSRR |= GPIO_BSRR_BS0;
        os_delay(delay);
        GPIOB->BSRR &= ~(GPIO_BSRR_BS0);
        GPIOB->BSRR |= GPIO_BSRR_BR0;
        os_delay(delay);
    }
}

void blink_pb1(void* argument) {
    uint32_t delay = *(uint32_t *)argument;
    while (true) {
        GPIOB->BSRR &= ~(GPIO_BSRR_BR1);
        GPIOB->BSRR |= GPIO_BSRR_BS1;
        os_delay(delay);
        GPIOB->BSRR &= ~(GPIO_BSRR_BS1);
        GPIOB->BSRR |= GPIO_BSRR_BR1;
        os_delay(delay);
    }
}

uint32_t argument1 = 250, argument2 = 333;

void main() {
    RCC->APB2ENR = RCC_APB2ENR_IOPBEN;
    GPIOB->CRL = GPIO_CRL_MODE1_1 | GPIO_CRL_MODE0_1;

    os_initialize();
    os_create(&blink_pb0, &argument1);
    os_create(&blink_pb1, &argument2);
    os_run();
}
```
