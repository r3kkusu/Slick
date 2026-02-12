#include "drivers/servo_math.h"

uint16_t servo_clamp_us(uint16_t us, uint16_t min_us, uint16_t max_us) {
    if (us < min_us) return min_us;
    if (us > max_us) return max_us;
    return us;
 
}