#include "kernel/scheduler.h"
#include "core/time.h"

#define MAX_TASKS 10

static task_t *g_tasks[MAX_TASKS];
static uint8_t g_count = 0;

static uint8_t due(uint32_t now, uint32_t *last, uint32_t period_ms) {
    if ((uint32_t)(now - *last) >= period_ms) {
        *last = now;
        return 1;
    }
    return 0;
}

void scheduler_init(void) {
    g_count = 0;
}

void scheduler_add(task_t *task) {
    if (g_count < MAX_TASKS) {
        g_tasks[g_count++] = task;
    }
}

void scheduler_run(void) {
    const uint32_t now = time_millis();

    for (uint8_t i = 0; i < g_count; i++) {
        task_t *t = g_tasks[i];
        if (due(now, &t->last_run_ms, t->period_ms)) {
            t->fn();
        }
    }
}