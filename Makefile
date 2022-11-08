sources = $(shell find source -type f -name "*.c")
objects = $(patsubst source/%.c,build/%.o,$(sources))
includes = -Iinclude
warnings = -Wall -Wextra -Wpedantic
flags = -c -Os -mcpu=cortex-m3 -mthumb $(includes) $(warnings)

.SILENT:
.PHONY: build pack clean

build: build/libos.a
	echo "size $(patsubst build/%,%,$<)"
	arm-none-eabi-size -t $<

pack:
	rm -rf dist
	mkdir -p dist/include
	mkdir -p dist/static
	cp -r include/os/core.h include/os/mutex.h dist/include
	cp build/libos.a dist/static

rebuild: build clean

clean:
	rm -rf build dist

build/libos.a: $(objects)
	echo "ar   $(patsubst build/%,%,$^)"
	arm-none-eabi-ar rcs $@ $^

build/%.o: source/%.c
	mkdir -p `dirname $@`
	echo "gcc  $(patsubst source/%,%,$<)"
	arm-none-eabi-gcc $(flags) -o $@ $<
