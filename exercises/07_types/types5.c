/*
 * types5
 *
 * Dividing two integers gives an integer, so 3 / 4 is 0. Make `percent`
 * come out as 75. Then, floating-point numbers are binary approximations:
 * 0.1 + 0.2 is not exactly 0.3. Implement `approx_equal` so that it treats
 * two doubles as equal when they differ by less than 1e-9.
 * See K&R 2.7 and Beej 15.
 */

#include <math.h>

#include "clings.h"

int approx_equal(double a, double b) {
    // TODO: compare with a tolerance; `fabs` gives an absolute value.
    return a == b;
}

int main(void) {
    // TODO: compute 3/4 as a percentage without losing the fraction.
    int percent = 3 / 4 * 100;

    // Don't change the tests!
    CHECK_EQ_INT(percent, 75);
    CHECK(approx_equal(0.1 + 0.2, 0.3));
    CHECK(!approx_equal(0.1, 0.2));
    return clings_summary();
}
