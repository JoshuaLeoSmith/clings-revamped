/*
 * recursion1
 *
 * `factorial` calls itself with a smaller `n` each time, but nothing ever
 * stops it, so it keeps going past 0 into negative numbers until the stack
 * runs out of room. AddressSanitizer then reports `stack-overflow`, which
 * almost always means recursion that never ends. Add the missing base case.
 * See K&R 4.10.
 */

#include "clings.h"

long factorial(int n) {
    // TODO: when should this stop calling itself? (0! is 1.)
    return n * factorial(n - 1);
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(factorial(5), 120);
    CHECK_EQ_INT(factorial(0), 1);

    return clings_summary();
}
