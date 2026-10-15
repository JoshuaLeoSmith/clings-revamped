// Fix: `if` tests a condition in parentheses and runs one branch or the other.
// In C any nonzero value counts as true; comparisons like `a > b` give 1 or 0.
/*
 * if1
 *
 * `bigger` should return the larger of its two arguments. Write its body
 * with `if` and `else`; don't use a library function.
 * See K&R 3.2 and Beej 3.3.1.
 */

#include "clings.h"

int bigger(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(bigger(10, 20), 20);
    CHECK_EQ_INT(bigger(30, 20), 30);
    CHECK_EQ_INT(bigger(-5, -10), -5);
    return clings_summary();
}
