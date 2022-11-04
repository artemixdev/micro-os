# MicroOS
![](https://github.com/artemixdev/micro-os/actions/workflows/build.yml/badge.svg)

MicroOS is a micro operating system designed for use in microcontrollers.
Its main purpose is to be minimalistic, transparent, and easy to use.
The system provides basic non-preemptive multitasking
allowing you to execute your tasks sequentially.
It also provides a simple API for interacting with the system at run time.
Currently, the focus is on ARM, but other popular architectures will be covered.

## Main features
- 🚀 Creating threads
- 🔀 Switching threads
- ⏰ Putting threads into sleep
- ⚙️ Synchronization primitives

## Supported architectures
- ✅ ARMv7-M and above
- ⚠️ ARMv6-M (partial support or not tested)

## Some details you need to know
The system uses `SysTick` timer for scheduling. Hence, you have to avoid using `SysTick` timer
for your purposes. Perhaps this behavior will be changed in the future,
but today there is no reason for this.

## Installation
The easiest and common way to is to add contents of `include` and `src` to your project.
Note that the configuration is set at compile time by defining certain symbols.
