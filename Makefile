sources = $(shell ls src | grep -E "\.c$$")
objects = $(patsubst %.c,%.o,$(sources))
cflags = -march=armv7-m -c -Iinclude -Wall -Wextra -Wpedantic

.SILENT:
.PHONY:

compile: $(objects)

clean:
	rm -rf build

%.o: src/%.c
	mkdir -p build
	echo "compile $(patsubst src/%,%,$<)"
	arm-none-eabi-gcc $(cflags) -o build/$@ $<
