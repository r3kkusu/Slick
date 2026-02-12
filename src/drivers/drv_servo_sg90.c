#include "drivers/drv_servo_sg90.h"
#include "hal/hal_gpio.h"
#include <util/delay.h>

static void delay_us_var(uint16_t us) {
    // Variable-safe: _delay_us(1) is constant; loop is variable
    while (us--) _delay_us(1);
}

void sg90_init(sg90_servo_t *s, uint8_t port_b_bit) {
    s->port_b_bit = port_b_bit;
    s->pulse_us = 1500;
    s->min_us = 900;
    s->max_us = 2100;

    hal_gpio_b_set_output(port_b_bit);
    hal_gpio_b_write(port_b_bit, GPIO_LOW);
}

void sg90_set_us(sg90_servo_t *s, uint16_t us) {
    if (us < s->min_us) us = s->min_us;
    if (us > s->max_us) us = s->max_us;
    s->pulse_us = us;
}

void sg90_frame_20ms(const sg90_servo_t *s) {
    hal_gpio_b_write(s->port_b_bit, GPIO_HIGH);
    delay_us_var(s->pulse_us);
    hal_gpio_b_write(s->port_b_bit, GPIO_LOW);
}