// Fix: each function handles its smallest input (exp == 0, n < 10) directly
// and otherwise reduces the problem by one step and calls itself.
/*
 * recursion2
 *
 * Write both functions recursively, without loops: each should answer its
 * smallest case directly and otherwise call itself on a smaller problem.
 * `power(base, exp)` is base multiplied by itself `exp` times (exp >= 0),
 * and `sum_digits(n)` adds up the decimal digits of `n` (n >= 0).
 * See K&R 4.10.
 */

#include "clings.h"

long power(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    return base * power(base, exp - 1);
}

int sum_digits(int n) {
    if (n < 10) {
        return n;
    }
    return n % 10 + sum_digits(n / 10);
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(power(2, 10), 1024);
    CHECK_EQ_INT(power(5, 0), 1);
    CHECK_EQ_INT(sum_digits(1234), 10);
    CHECK_EQ_INT(sum_digits(0), 0);

    return clings_summary();
}
