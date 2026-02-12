#pragma once
#include <stdint.h>

// Initialize 1ms tick using Timer2 compare interrupt
void hal_tick_init_1ms_timer2(void);

// Read current milliseconds since boot
uint32_t hal_tick_millis(void);