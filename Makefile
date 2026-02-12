# Makefile for ATmega328P (C) + USBasp
#
# Project structure:
#   config/
#   include/
#   src/ (can contain subfolders: hal/, drivers/, kernel/, app/)
#   tests/
#   build/   (generated)
#
# Usage:
#   make                # builds build/main.elf + build/main.hex
#   make flash          # flashes using USBasp (uses B?=200 by default)
#   make flash B=10     # faster ISP clock (if stable)
#   make upload         # build + flash (one command)
#   make test           # runs host-based unit tests (PC)
#   make fuses-read     # read lfuse/hfuse/efuse
#   make fuses-16mhz    # set fuses for external 16MHz crystal (UNO-like)
#   make clean

MCU      := atmega328p

# Once you set fuses for external 16MHz crystal, keep F_CPU at 16000000UL.
F_CPU    := 16000000UL

TARGET    := main
BUILD_DIR := build

CC        := avr-gcc
OBJCOPY   := avr-objcopy

# Include paths for clean architecture structure
CFLAGS := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall \
          -Iinclude -Iconfig

PROGRAMMER := usbasp
PART       := m328p

# Default to slow/safer ISP clock; override with B=10 if you want
B ?= 200

# Fuse values (UNO-like) for ATmega328P @ 16MHz external crystal
LFUSE_16MHZ := 0xFF
HFUSE_16MHZ := 0xDE
EFUSE_16MHZ := 0x05

# Recursively pick up all .c files under src/
SRCS := $(shell find src -name '*.c')
ELF  := $(BUILD_DIR)/$(TARGET).elf
HEX  := $(BUILD_DIR)/$(TARGET).hex

# ---------------- Host-based unit tests (runs on your PC) ----------------
HOST_CC  := cc
TEST_BIN := $(BUILD_DIR)/tests

# Only include "pure" logic modules here (no AVR register code)
TEST_SRCS := $(shell find tests -name '*.c') \
             src/kernel/scheduler_logic.c \
             src/drivers/servo_math.c

.PHONY: all clean flash upload test fuses-read fuses-16mhz

all: $(HEX)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ELF): $(SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

flash: $(HEX)
	avrdude -c $(PROGRAMMER) -p $(PART) -B $(B) -U flash:w:$(HEX)

# Build + flash (override bitclock if needed: make upload B=10)
upload: all flash

# Run unit tests on host (PC)
test: $(BUILD_DIR)
	$(HOST_CC) -O0 -g -Wall -Iinclude -Iconfig -o $(TEST_BIN) $(TEST_SRCS)
	./$(TEST_BIN)

# Read fuses (safe)
fuses-read:
	avrdude -c $(PROGRAMMER) -p $(PART) -B 200 \
	  -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

# Set fuses for external 16MHz crystal (be sure your 16MHz crystal + caps are wired correctly)
fuses-16mhz:
	avrdude -c $(PROGRAMMER) -p $(PART) -B 200 \
	  -U lfuse:w:$(LFUSE_16MHZ):m \
	  -U hfuse:w:$(HFUSE_16MHZ):m \
	  -U efuse:w:$(EFUSE_16MHZ):m

clean:
	rm -rf $(BUILD_DIR)