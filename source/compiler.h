#pragma once

#define NAKED __attribute__((naked))
#define UNUSED __attribute__((unused))

#define USED(var) (void)var
