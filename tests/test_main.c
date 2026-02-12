#include <stdio.h>

int test_scheduler(void);
int test_servo(void);

int main(void) {
    int fails = 0;

    fails += test_scheduler();
    fails += test_servo();

    if (fails == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    }

    printf("TESTS FAILED: %d\n", fails);
    return 1;
}