#pragma once
#include <stdint.h>

// Pure function: returns 1 if task is due, updates last_run if due.
// Safe with uint32 wrap-around.
uint8_t scheduler_due(uint32_t now, uint32_t *last_run_ms, uint32_t period_ms);