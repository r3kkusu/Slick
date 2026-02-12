#pragma once
#include <stdint.h>

// Board clock
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// LED: PB0 (DIP pin 14)
#define BOARD_LED_PORT_B_BIT   0

// Servo signal: PB1 (OC1A, DIP pin 15)
#define BOARD_SERVO_PORT_B_BIT 1

// Servo frame rate
#define BOARD_SERVO_FRAME_MS   20u  // 50Hz