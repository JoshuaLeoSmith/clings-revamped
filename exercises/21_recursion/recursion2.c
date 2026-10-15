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
    // TODO: what is base to the power 0? How does base^exp relate to base^(exp - 1)?
    (void)base;
    (void)exp;
    return 0;
}

int sum_digits(int n) {
    // TODO: `n % 10` is the last digit and `n / 10` is the rest of the number.
    (void)n;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(power(2, 10), 1024);
    CHECK_EQ_INT(power(5, 0), 1);
    CHECK_EQ_INT(sum_digits(1234), 10);
    CHECK_EQ_INT(sum_digits(0), 0);

    return clings_summary();
}
