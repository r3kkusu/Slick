#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

static volatile uint32_t g_ms = 0;

ISR(TIMER2_COMPA_vect) {
    g_ms++;
}

static uint32_t millis(void) {
    uint32_t m;
    uint8_t s = SREG;
    cli();
    m = g_ms;
    SREG = s;
    return m;
}

static void timer2_init_1ms(void) {
    // Timer2 CTC, prescaler 64:
    // 16MHz/64 = 250kHz => 4us/tick, 1ms => 250 ticks => OCR2A = 249
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS22);   // prescaler 64
    OCR2A  = 249;
    TCNT2  = 0;
    TIMSK2 = (1 << OCIE2A);
}

// Variable-safe delay in microseconds (works because _delay_us(1) is constant)
static void delay_us_var(uint16_t us) {
    while (us--) _delay_us(1);
}

// --- Servo pulse generator (called once every 20ms by scheduler) ---
static uint16_t g_servo_us = 1500;

static void servo_pulse_frame(void) {
    PORTB |= _BV(PORTB1);
    delay_us_var(g_servo_us);     // <-- variable-safe
    PORTB &= ~_BV(PORTB1);
}

static uint8_t due(uint32_t now, uint32_t *last, uint32_t period_ms) {
    if ((uint32_t)(now - *last) >= period_ms) {
        *last = now;
        return 1;
    }
    return 0;
}

int main(void) {
    // LED PB0 (DIP pin 14)
    DDRB |= _BV(DDB0);

    // Servo PB1 (DIP pin 15)
    DDRB |= _BV(DDB1);
    PORTB &= ~_BV(PORTB1);

    timer2_init_1ms();
    sei();

    uint32_t last_led = 0;
    uint32_t last_servo_step = 0;
    uint32_t last_servo_frame = 0;

    const uint16_t pos_us[] = {1000, 1500, 2000, 1500};
    uint8_t pos_i = 0;

    while (1) {
        uint32_t now = millis();

        // 50Hz servo signal: one pulse every 20ms
        if (due(now, &last_servo_frame, 20)) {
            servo_pulse_frame();
        }

        // Blink LED every 1000ms
        if (due(now, &last_led, 1000)) {
            PORTB ^= _BV(PORTB0);
        }

        // Change servo position every 1000ms
        if (due(now, &last_servo_step, 1000)) {
            g_servo_us = pos_us[pos_i];
            pos_i = (uint8_t)((pos_i + 1) & 3);
        }
    }
}