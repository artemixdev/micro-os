#include "compiler.h"

extern void entry();

NAKED int main() {
    entry();
    return 0;
}
