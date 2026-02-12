#include <stdint.h>
#include <stdio.h>
#include "drivers/servo_math.h"

#define ASSERT_EQ(a,b) do { if ((a) != (b)) { \
    printf("FAIL: %s:%d: expected %u got %u\n", __FILE__, __LINE__, (unsigned)(b), (unsigned)(a)); return 1; } } while(0)

int test_servo(void) {
    ASSERT_EQ(servo_clamp_us(500, 900, 2100), 900);
    ASSERT_EQ(servo_clamp_us(900, 900, 2100), 900);
    ASSERT_EQ(servo_clamp_us(1500, 900, 2100), 1500);
    ASSERT_EQ(servo_clamp_us(2100, 900, 2100), 2100);
    ASSERT_EQ(servo_clamp_us(2500, 900, 2100), 2100);

    printf("PASS: servo\n");
    return 0;
}