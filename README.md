# ATmega328P Build & Flash (Makefile Workflow)

This project uses a simple **Makefile** with the following structure:

```
project/
├─ Makefile
├─ src/
│  └─ main.c
└─ build/          # generated outputs
```

## Prerequisites

Install the AVR toolchain and avrdude.

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

## Build (using Makefile)

From the project root (where the `Makefile` is):

### Compile + generate HEX

```bash
make
```

Outputs:
- `build/main.elf`
- `build/main.hex`

### Clean build outputs

```bash
make clean
```

---

## Flash (USBasp)

### Flash using default ISP bitclock (B=10)

```bash
make flash
```

### Flash using a slower ISP bitclock (recommended for unstable/slow targets)

```bash
make flash B=200
# or
make flashslow
```

### One command: build + flash

```bash
make upload
# or safer:
make flashslow
```

---

## Manual commands (no Makefile)

If you prefer to run commands manually, note the **new paths**:

### Compile (C)

```bash
avr-gcc \
  -mmcu=atmega328p \
  -DF_CPU=16000000UL \
  -Os \
  -Wall \
  -o build/main.elf \
  src/main.c
```

### Convert ELF to HEX

```bash
avr-objcopy -O ihex build/main.elf build/main.hex
```

### Flash via USBasp

```bash
avrdude -c usbasp -p m328p -B 10 -U flash:w:build/main.hex
# slower:
avrdude -c usbasp -p m328p -B 200 -U flash:w:build/main.hex
```
