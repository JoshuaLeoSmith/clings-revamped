/*
 * types2
 *
 * An `int` has a largest value, `INT_MAX` from <limits.h>. Going past it is
 * undefined behaviour in C, and the sanitizer stops the program when it
 * happens. Change the type of `value` so that it can hold `INT_MAX + 1`.
 * See K&R 2.2 and Beej 14.
 */

#include <limits.h>

#include "clings.h"

int main(void) {
    // TODO: pick a wider type for `value`.
    int value = INT_MAX;
    value = value + 1;

    // Don't change the tests!
    CHECK_EQ_INT(value, 2147483648LL);
    return clings_summary();
}
