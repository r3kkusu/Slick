# Makefile for ATmega328P (C only) + USBasp
#
# Project structure:
#   src/main.c
#   build/   (generated)
#
# Usage:
#   make                # builds build/main.elf + build/main.hex
#   make flash          # flashes using USBasp (uses B?=10 by default)
#   make flash B=200    # slower ISP clock
#   make upload         # build + flash (one command)
#   make flashslow      # build + flash with B=200
#   make fuses-read     # read lfuse/hfuse/efuse
#   make fuses-16mhz    # set fuses for external 16MHz crystal (UNO-like)
#   make clean

MCU      := atmega328p

# NOTE:
# If your measured delay is ~16x longer, your chip is likely running ~1MHz.
# Once you set fuses for external 16MHz crystal, keep F_CPU at 16000000UL.
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

# Fuse values (UNO-like) for ATmega328P @ 16MHz external crystal
LFUSE_16MHZ := 0xFF
HFUSE_16MHZ := 0xDE
EFUSE_16MHZ := 0x05

.PHONY: all clean flash upload flashslow fuses-read fuses-16mhz

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