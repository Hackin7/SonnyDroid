include config


CFLAGS += -std=gnu99 -Wall -W -O2 -g  -Wmissing-declarations -ffast-math
CFLAGS += -DRENDER_$(RENDER)
LDFLAGS := -g

ifeq ($(USE_DEBUG),yes)
CFLAGS += -DUSE_DEBUG
endif


# Default targets.
.PHONY: all
all: un-disas uuu-$(PLATFORM)

# The disassembler.
un-disas: un-disas.o disas.o

# The emulator.
uuu-$(PLATFORM): uuu-%: uuu-%.o platform-%.o \
                 disas.o emu.o timer.o video.o render.o render-$(RENDER).o \
                 audio.o io.o i2c-bus.o i2c-eeprom.o board.o \
                 board-VII.o board-W60.o board-WAL.o board-BAT.o board-V_X.o board-dummy.o

# SDL needs special compiler flags and some libraries.
platform-sdl.o uuu-sdl.o: CFLAGS += $(shell sdl-config --cflags)
uuu-sdl: LDFLAGS += $(shell sdl-config --libs)

uuu-sdl: LDFLAGS += -lGL

# Laziness rules, and lazy rules rule most of all.
*.o: *.h Makefile config

# Clean up.
.PHONY: clean
clean:
	rm -f un-disas un-disas.o disas.o
	rm -f emu.o timer.o video.o audio.o io.o
	rm -f i2c-bus.o i2c-eeprom.o
	rm -f board.o board-VII.o board-W60.o board-WAL.o board-BAT.o board-V_X.o board-dummy.o
	rm -f render.o render-gl.o render-soft.o
	rm -f uuu-sdl uuu-sdl.o platform-sdl.o dialog-cocoa.o
