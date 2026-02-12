#include "hal/hal_tick.h"
#include "core/time.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static volatile uint32_t g_ms = 0;

ISR(TIMER2_COMPA_vect) {
    g_ms++;
}

void hal_tick_init_1ms_timer2(void) {
    // Timer2 CTC, prescaler 64:
    // 16MHz/64 = 250kHz => 4us/tick, 1ms => 250 ticks => OCR2A=249
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS22);     // prescaler 64
    OCR2A  = 249;
    TCNT2  = 0;
    TIMSK2 = (1 << OCIE2A);
}

uint32_t hal_tick_millis(void) {
    uint32_t m;
    uint8_t sreg = SREG;
    cli();
    m = g_ms;
    SREG = sreg;
    return m;
}

// core/time.h implementation uses hal_tick
uint32_t time_millis(void) {
    return hal_tick_millis();
}