// Fix: `do ... while` tests its condition after the body, so the body runs at
// least once; that gives 0 its one digit.
/*
 * loops3
 *
 * This `count_digits` works for 12345, but says 0 has no digits at all,
 * because a `while` loop checks its condition before the first pass. C has
 * a loop that runs its body once before checking: rewrite this one with it.
 * See K&R 3.6 and Beej 3.3.3.
 */

#include "clings.h"

int count_digits(int n) {
    int count = 0;
    do {
        count++;
        n /= 10;
    } while (n > 0);
    return count;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(count_digits(0), 1);
    CHECK_EQ_INT(count_digits(12345), 5);
    return clings_summary();
}
