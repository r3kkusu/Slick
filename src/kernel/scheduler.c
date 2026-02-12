#include "kernel/scheduler.h"
#include "kernel/scheduler_logic.h"
#include "core/time.h"

#define MAX_TASKS 10

static task_t *g_tasks[MAX_TASKS];
static uint8_t g_count = 0;

void scheduler_init(void) { g_count = 0; }

void scheduler_add(task_t *task) {
    if (g_count < MAX_TASKS) g_tasks[g_count++] = task;
}

void scheduler_run(void) {
    const uint32_t now = time_millis();

    for (uint8_t i = 0; i < g_count; i++) {
        task_t *t = g_tasks[i];
        if (scheduler_due(now, &t->last_run_ms, t->period_ms)) {
            t->fn();
        }
    }
}