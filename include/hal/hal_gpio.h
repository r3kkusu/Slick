#pragma once
#include <stdint.h>

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_level_t;

void hal_gpio_b_set_output(uint8_t bit);
void hal_gpio_b_write(uint8_t bit, gpio_level_t level);
void hal_gpio_b_toggle(uint8_t bit);