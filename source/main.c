#include "compiler.h"
#include "kernel.h"

extern void entry();

NAKED int main() {
    asm volatile (
        "mov r0, %[address] \n"
        "mov sp, r0"
        :: [address] "i" (MAIN_STACK_ADDRESS)
    );

    entry();
    return 0;
}
