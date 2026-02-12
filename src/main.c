/*
 * ATmega328P: LED blink (PB0) + SG90 Servo control (PB1/OC1A)
 * - 16MHz external crystal
 * - Servo: 50Hz PWM using Timer1 (hardware PWM, stable)
 *
 * Wiring:
 * - LED: PB0 (DIP pin 14) -> 220Ω -> LED anode, LED cathode -> GND
 * - Servo signal: PB1/OC1A (DIP pin 15) -> servo signal (orange)
 * - Servo power: external 5V battery/regulator (red=+5V, brown=GND)
 * - IMPORTANT: servo GND must connect to ATmega GND
 *
 * Build with: -DF_CPU=16000000UL
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

// Timer1 config:
// Prescaler 8 => timer tick = 16MHz/8 = 2MHz => 0.5us per tick
// 20ms period = 20000us => 20000 / 0.5 = 40000 ticks
#define SERVO_TOP_TICKS 40000U

// Pulse widths for SG90 (typical):
// 1.0ms = 1000us => 2000 ticks
// 1.5ms = 1500us => 3000 ticks
// 2.0ms = 2000us => 4000 ticks
#define SERVO_MIN_TICKS 2000U
#define SERVO_MID_TICKS 3000U
#define SERVO_MAX_TICKS 4000U

static void servo_init(void) {
    // PB1 (OC1A) as output
    DDRB |= _BV(DDB1);

    // Fast PWM, mode 14: WGM13:WGM10 = 1110 (TOP = ICR1)
    // Clear OC1A on compare match, set at BOTTOM (non-inverting)
    TCCR1A = _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11); // prescaler = 8

    ICR1 = SERVO_TOP_TICKS;      // 20ms period
    OCR1A = SERVO_MID_TICKS;     // start at center (1.5ms)
}

static void servo_set_ticks(uint16_t ticks) {
    // clamp to safe range
    if (ticks < SERVO_MIN_TICKS) ticks = SERVO_MIN_TICKS;
    if (ticks > SERVO_MAX_TICKS) ticks = SERVO_MAX_TICKS;
    OCR1A = ticks;
}

int main(void) {
    // LED on PB0
    DDRB |= _BV(DDB0);
    PORTB &= ~_BV(PORTB0);

    // Servo on PB1/OC1A
    servo_init();

    while (1) {
        // LED blink
        PORTB ^= _BV(PORTB0);

        // Servo move pattern (each step 1 second)
        servo_set_ticks(SERVO_MIN_TICKS); // ~0°
        _delay_ms(1000);

        servo_set_ticks(SERVO_MID_TICKS); // ~90°
        _delay_ms(1000);

        servo_set_ticks(SERVO_MAX_TICKS); // ~180°
        _delay_ms(1000);

        servo_set_ticks(SERVO_MID_TICKS); // ~90°
        _delay_ms(1000);
    }
}