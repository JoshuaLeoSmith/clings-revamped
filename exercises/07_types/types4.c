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
    // TODO: make the loop able to stop.
    for (unsigned i = 10; i >= 0; i--) {
        sum += i;
    }
    return sum;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(countdown_sum(), 55);
    return clings_summary();
}
