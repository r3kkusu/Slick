#pragma once
#include <stdint.h>

typedef void (*task_fn_t)(void);

typedef struct {
    task_fn_t fn;
    uint32_t  period_ms;
    uint32_t  last_run_ms;
} task_t;

void scheduler_init(void);
void scheduler_add(task_t *task);
void scheduler_run(void);