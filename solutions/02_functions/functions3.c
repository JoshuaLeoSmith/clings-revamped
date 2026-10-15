// Fix: C never returns the last expression implicitly; you need `return`.
// Falling off the end of a non-void function leaves the caller with garbage.
/*
 * functions3
 *
 * A function declared to return `int` must hand a value back to its caller.
 * `square` computes the right number and then drops it. Fix `square`.
 * Warnings count as errors in this exercise.
 * See K&R 1.8 and 4.1, and Beej 4.
 */

#include "clings.h"

int square(int n) {
    return n * n;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(square(4), 16);
    CHECK_EQ_INT(square(-3), 9);
    return clings_summary();
}
