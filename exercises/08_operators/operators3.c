/*
 * operators3
 *
 * The conditional operator `cond ? a : b` is an expression: it gives `a`
 * when `cond` is true and `b` otherwise. Implement `clamp`, which keeps
 * `value` within `lo` to `hi`, and `sign`, which gives -1, 0 or 1, each as a
 * single `return` using `?:`.
 * See K&R 2.11 and Beej 3.2.
 */

#include "clings.h"

int clamp(int value, int lo, int hi) {
    // TODO: return lo if value is below it, hi if above, else value.
    (void)value;
    (void)lo;
    (void)hi;
    return 0;
}

int sign(int x) {
    // TODO: return -1 for negative x, 1 for positive x, 0 for zero.
    (void)x;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(clamp(5, 0, 10), 5);
    CHECK_EQ_INT(clamp(-3, 0, 10), 0);
    CHECK_EQ_INT(clamp(42, 0, 10), 10);
    CHECK_EQ_INT(sign(-7), -1);
    CHECK_EQ_INT(sign(0), 0);
    CHECK_EQ_INT(sign(9), 1);
    return clings_summary();
}
