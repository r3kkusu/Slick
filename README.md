# Humanoid-FW (ATmega328P) — Build, Flash, and Test

Firmware scaffold for an **ATmega328P @ 16MHz** with a clean, scalable structure (HAL / drivers / kernel / app).
Current demo behavior:
- **LED blink** (PB0 / DIP pin 14)
- **SG90 servo pulse frames** (PB1 / DIP pin 15) scheduled at **50Hz**

---

## Project Structure

```
humanoid-fw/
├─ Makefile
├─ README.md
├─ .gitignore
├─ docs/
│  ├─ wiring.md
│  ├─ timing.md
│  └─ conventions.md
├─ config/
│  └─ board_atmega328p.h
├─ include/
│  ├─ core/
│  ├─ hal/
│  ├─ drivers/
│  ├─ kernel/
│  └─ app/
├─ src/
│  ├─ main.c
│  ├─ core/
│  ├─ hal/
│  ├─ drivers/
│  ├─ kernel/
│  └─ app/
└─ tests/
   └─ (host-based unit tests)
```

**Build outputs** go to:
- `build/main.elf`
- `build/main.hex`

---

## Prerequisites

Install AVR toolchain + avrdude + make.

### macOS (Homebrew)

```bash
brew install avr-gcc avrdude
```

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install gcc-avr avr-libc avrdude make
```

---

## Build (Makefile)

From the project root (where the `Makefile` is):

### Compile + generate HEX

```bash
make
```

### Clean build outputs

```bash
make clean
```

---

## Flash (USBasp)

The Makefile defaults to a safer/slower ISP bitclock:
- Default: `B=200`

### Flash

```bash
make flash
```

### Flash faster (if stable)

```bash
make flash B=10
```

### One command: build + flash

```bash
make upload
# or override bitclock:
make upload B=10
```

---

## Read / Set Fuses (16MHz Crystal)

If your delays are ~16× too slow, the chip may be running at ~1MHz (factory default).

### Read fuses (safe)

```bash
make fuses-read
```

### Set fuses for external 16MHz crystal (UNO-like)

> Only do this if your **16MHz crystal + caps** are wired correctly.

```bash
make fuses-16mhz
```

---

## Unit Tests (Host-based)

Unit tests run on your **computer**, not on the ATmega. They should cover **pure logic**
(scheduler math, clamping, control math), not AVR register code.

```bash
make test
```

---

## Manual Commands (No Makefile)

These are shown for reference. The Makefile is the recommended workflow.

### Compile all sources (example)

```bash
avr-gcc \
  -mmcu=atmega328p \
  -DF_CPU=16000000UL \
  -Os -Wall \
  -Iinclude -Iconfig \
  -o build/main.elf \
  $(find src -name '*.c')
```

### Convert ELF to HEX

```bash
avr-objcopy -O ihex build/main.elf build/main.hex
```

### Flash via USBasp

```bash
avrdude -c usbasp -p m328p -B 200 -U flash:w:build/main.hex
# faster:
avrdude -c usbasp -p m328p -B 10 -U flash:w:build/main.hex
```

---

## Notes (Servo Power)

- Power the **servo from an external battery/regulator** (5V recommended).
- **Common ground is required**: servo GND must connect to ATmega GND.
- Add a **220–470µF capacitor** across servo +5V/GND near the servo to reduce resets/jitter.

---

## Next Steps (Scaling to Humanoid)

- Move from single-servo pulse frames to a multi-servo strategy:
  - Recommended: **PCA9685** (I2C servo driver) for many servos.
- Add IMU driver + control loop task (fixed-rate).
- Add safety: watchdog, battery monitoring, servo disable on fault.
