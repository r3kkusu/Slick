#pragma once
#include <stdint.h>

typedef struct {
    uint8_t  port_b_bit;   // which PBx bit to use for signal
    uint16_t pulse_us;     // current pulse width
    uint16_t min_us;       // clamp
    uint16_t max_us;       // clamp
} sg90_servo_t;

void sg90_init(sg90_servo_t *s, uint8_t port_b_bit);
void sg90_set_us(sg90_servo_t *s, uint16_t us);

// Call once per 20ms frame (50Hz). Generates ONE pulse frame.
void sg90_frame_20ms(const sg90_servo_t *s);