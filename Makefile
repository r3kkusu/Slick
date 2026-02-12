# Makefile for ATmega328P (C only) + USBasp
#
# Project structure:
#   src/main.c
#   build/   (generated)
#
# Usage:
#   make              # builds build/main.elf + build/main.hex
#   make flash        # flashes using USBasp (uses B?=10 by default)
#   make flash B=200  # slower ISP clock
#   make upload       # build + flash (one command)
#   make flashslow    # build + flash with B=200
#   make clean

MCU      := atmega328p
F_CPU    := 16000000UL

TARGET   := main
SRC_DIR  := src
BUILD_DIR:= build

SRC      := $(SRC_DIR)/$(TARGET).c
ELF      := $(BUILD_DIR)/$(TARGET).elf
HEX      := $(BUILD_DIR)/$(TARGET).hex

CC       := avr-gcc
OBJCOPY  := avr-objcopy

CFLAGS   := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

PROGRAMMER := usbasp
PART       := m328p
B ?= 10

.PHONY: all clean flash upload flashslow

all: $(HEX)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ELF): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

flash: $(HEX)
	avrdude -c $(PROGRAMMER) -p $(PART) -B $(B) -U flash:w:$(HEX)

# Build + flash (override bitclock if needed: make upload B=200)
upload: all flash

# Build + flash using a slow ISP clock (good for unstable targets)
flashslow: all
	$(MAKE) flash B=200

clean:
	rm -rf $(BUILD_DIR)