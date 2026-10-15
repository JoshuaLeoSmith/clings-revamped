// Fix: an unsigned value is never below 0, so `i >= 0` is always true and
// `i--` at 0 wraps to UINT_MAX. A signed `int` index can reach -1 and stop.
/*
 * types4
 *
 * `countdown_sum` should add up 10, 9, ..., 1, 0. An `unsigned` value can
 * never be negative: decrementing 0 wraps around to a huge number, so this
 * loop would never end. The compiler notices the condition is always true and
 * warns, and warnings count as errors here.
 * See K&R 2.2 and Beej 14.
 */

#include "clings.h"

int countdown_sum(void) {
    int sum = 0;
    for (int i = 10; i >= 0; i--) {
        sum += i;
    }
    return sum;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(countdown_sum(), 55);
    return clings_summary();
}
