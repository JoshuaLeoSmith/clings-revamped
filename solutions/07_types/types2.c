// Fix: signed overflow is undefined behaviour; `long long` is at least 64 bits,
// so INT_MAX + 1 fits. The addition is done in long long because `value` is one.
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
    long long value = INT_MAX;
    value = value + 1;

    // Don't change the tests!
    CHECK_EQ_INT(value, 2147483648LL);
    return clings_summary();
}
