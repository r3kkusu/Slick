#include "app/app.h"
#include "board_atmega328p.h"
#include "kernel/scheduler.h"
#include "hal/hal_gpio.h"
#include "drivers/drv_servo_sg90.h"

static sg90_servo_t g_servo;

// --- Tasks ---
static void task_servo_frame(void);
static void task_led_blink(void);
static void task_servo_step(void);

// 50Hz servo frame (must be continuous)
static task_t g_task_servo_frame = {
    .fn = task_servo_frame,
    .period_ms = BOARD_SERVO_FRAME_MS,
    .last_run_ms = 0
};

// LED every 1s
static task_t g_task_led = {
    .fn = task_led_blink,
    .period_ms = 1000,
    .last_run_ms = 0
};

// change servo position every 1s
static task_t g_task_servo_step = {
    .fn = task_servo_step,
    .period_ms = 1000,
    .last_run_ms = 0
};

void app_init(void) {
    // LED PB0
    hal_gpio_b_set_output(BOARD_LED_PORT_B_BIT);
    hal_gpio_b_write(BOARD_LED_PORT_B_BIT, GPIO_LOW);

    // Servo PB1
    sg90_init(&g_servo, BOARD_SERVO_PORT_B_BIT);
    sg90_set_us(&g_servo, 1500);
}

void app_register_tasks(void) {
    scheduler_add(&g_task_servo_frame);
    scheduler_add(&g_task_led);
    scheduler_add(&g_task_servo_step);
}

static void task_servo_frame(void) {
    sg90_frame_20ms(&g_servo);
}

static void task_led_blink(void) {
    hal_gpio_b_toggle(BOARD_LED_PORT_B_BIT);
}

static void task_servo_step(void) {
    static const uint16_t pos_us[] = {1000, 1500, 2000, 1500};
    static uint8_t i = 0;

    sg90_set_us(&g_servo, pos_us[i]);
    i = (uint8_t)((i + 1) & 3);
}