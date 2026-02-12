/*
 * ATmega328P blink LED every 1 second (USBasp/ISP friendly)
 * LED pin: PB0 (ATmega DIP pin 14)  <-- avoids SCK (PB5) flicker during programming
 *
 * Build with: -DF_CPU=16000000UL
 */

#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

static void delay_1s_approx(void) {
    // Rough ~1 second delay at 16 MHz (busy-wait)
    for (volatile uint32_t i = 0; i < 5333333UL; i++) {
        __asm__ __volatile__("nop");
    }
}

int main(void) {
    // PB0 as output (ATmega DIP pin 14)
    DDRB |= (1 << DDB0);

    while (1) {
        PORTB ^= (1 << PORTB0);  // toggle LED
        delay_1s_approx();
    }
}