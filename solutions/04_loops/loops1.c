// Fix: `i < n` stops before n; `i <= n` includes it. Off-by-one errors at a
// loop's boundary are among the most common bugs in C.
/*
 * loops1
 *
 * `sum_to(n)` should add up 1 + 2 + ... + n, but its answers are a little
 * short. A `for` loop has three parts: start, condition, and step; look
 * closely at where this one stops.
 * See K&R 1.3 and 3.5, and Beej 3.3.4.
 */

#include "clings.h"

int sum_to(int n) {
    int total = 0;
    for (int i = 1; i <= n; i++) {
        total += i;
    }
    return total;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(sum_to(10), 55);
    CHECK_EQ_INT(sum_to(1), 1);
    CHECK_EQ_INT(sum_to(0), 0);
    return clings_summary();
}
