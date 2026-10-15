/*
 * variables3
 *
 * The type of a variable decides what it can hold. C converts values
 * silently when you assign them, even when that throws information away.
 * `ratio` should hold three quarters, 0.75. Fix it without changing the tests.
 * See K&R 2.2 and 2.7, and Beej 3.1.
 */

#include "clings.h"

int main(void) {
    // TODO: `ratio` comes out as 0. Why?
    int ratio = 3.0 / 4.0;

    // Don't change the tests!
    CHECK_EQ_DBL(ratio, 0.75);
    return clings_summary();
}
