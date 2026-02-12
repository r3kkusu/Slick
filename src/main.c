/*
 * ATmega328P blink LED (PB0 / DIP pin 14)
 * 16MHz external crystal
 *
 * Build with: -DF_CPU=16000000UL
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= _BV(DDB0);        // PB0 output
    PORTB &= ~_BV(PORTB0);    // LED OFF initially

    while (1) {
        PORTB ^= _BV(PORTB0); // toggle LED
        _delay_ms(1000);
    }
}