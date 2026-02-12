#include "hal/hal_gpio.h"
#include <avr/io.h>

void hal_gpio_b_set_output(uint8_t bit) {
    DDRB |= (1 << bit);
}

void hal_gpio_b_write(uint8_t bit, gpio_level_t level) {
    if (level == GPIO_HIGH) PORTB |= (1 << bit);
    else                    PORTB &= ~(1 << bit);
}

void hal_gpio_b_toggle(uint8_t bit) {
    PORTB ^= (1 << bit);
}