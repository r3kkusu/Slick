#include "kernel/scheduler_logic.h"

uint8_t scheduler_due(uint32_t now, uint32_t *last_run_ms, uint32_t period_ms) {
    if ((uint32_t)(now - *last_run_ms) >= period_ms) {
        *last_run_ms = now;
        return 1;
    }
    return 0;
}