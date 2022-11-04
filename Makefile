sources = $(shell find src -type f -name "*.c")
objects = $(patsubst src/%.c,build/%.o,$(sources))
cflags = -march=armv7-m -c -Iinclude -Wall -Wextra -Wpedantic

.SILENT:
.PHONY: compile clean

compile: $(objects)

clean:
	rm -rf build

build/%.o: src/%.c
	mkdir -p `dirname $@`
	echo "compile $(patsubst src/%,%,$<)"
	arm-none-eabi-gcc $(cflags) -o $@ $<
