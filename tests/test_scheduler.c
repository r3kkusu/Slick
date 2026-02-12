#include <stdint.h>
#include <stdio.h>
#include "kernel/scheduler_logic.h"

#define ASSERT_TRUE(x) do { if (!(x)) { \
    printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #x); return 1; } } while(0)

int test_scheduler(void) {
    // Basic due logic
    uint32_t last = 0;

    ASSERT_TRUE(scheduler_due(0, &last, 20) == 0);
    ASSERT_TRUE(last == 0);

    ASSERT_TRUE(scheduler_due(19, &last, 20) == 0);
    ASSERT_TRUE(last == 0);

    ASSERT_TRUE(scheduler_due(20, &last, 20) == 1);
    ASSERT_TRUE(last == 20);

    ASSERT_TRUE(scheduler_due(39, &last, 20) == 0);
    ASSERT_TRUE(last == 20);

    ASSERT_TRUE(scheduler_due(40, &last, 20) == 1);
    ASSERT_TRUE(last == 40);

    // Wrap-around safety test (uint32 overflow behavior)
    last = 0xFFFFFFF0u; // near overflow
    ASSERT_TRUE(scheduler_due(0xFFFFFFF5u, &last, 20) == 0);
    ASSERT_TRUE(scheduler_due(0x00000010u, &last, 20) == 1); // wrapped and enough time

    printf("PASS: scheduler\n");
    return 0;
}