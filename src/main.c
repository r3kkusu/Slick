#include <avr/interrupt.h>

#include "hal/hal_tick.h"
#include "kernel/scheduler.h"
#include "app/app.h"

int main(void) {
    hal_tick_init_1ms_timer2();
    scheduler_init();

    app_init();
    app_register_tasks();

    sei();

    while (1) {
        scheduler_run();
        // idle: later you can add sleep, event queue, UART processing, etc.
    }
}