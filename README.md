## Compile (ASSEMBLY)

```bash
avr-gcc -mmcu=atmega328p -x assembler -o main.elf main.asm
```

## Compile (C)

```bash
avr-gcc \
  -mmcu=atmega328p \
  -DF_CPU=16000000UL \
  -Os \
  -Wall \
  -o main.elf \
  main.c
```

## Compile (C++)

```bash
avr-g++ \
  -mmcu=atmega328p \
  -DF_CPU=16000000UL \
  -Os \
  -Wall \
  -fno-exceptions \
  -fno-rtti \
  -o main.elf \
  main.cpp
```

## Convert to HEX

```bash
avr-objcopy -O ihex main.elf main.hex
```

### USBasp

```bash
avrdude -c usbasp -p m328p -B 10 -U flash:w:main.hex
```



