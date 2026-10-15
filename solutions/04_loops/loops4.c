// Fix: `break` leaves the loop at once, so the first match is kept. `continue`
// skips the rest of one pass and moves on to the next value of `i`.
/*
 * loops4
 *
 * `first_divisible_by_7_and_11` should return the first number divisible by
 * both 7 and 11, but the loop keeps going after it finds one. Then implement
 * `sum_skipping_multiples_of_3`, using `continue` to skip numbers.
 * See K&R 3.7 and Beej 3.3.4.
 */

#include "clings.h"

int first_divisible_by_7_and_11(void) {
    int found = 0;
    for (int i = 1; i <= 1000; i++) {
        if (i % 7 == 0 && i % 11 == 0) {
            found = i;
            break;
        }
    }
    return found;
}

// Returns 1 + 2 + ... + n, leaving out the multiples of 3.
int sum_skipping_multiples_of_3(int n) {
    int total = 0;
    for (int i = 1; i <= n; i++) {
        if (i % 3 == 0)
            continue;
        total += i;
    }
    return total;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(first_divisible_by_7_and_11(), 77);
    CHECK_EQ_INT(sum_skipping_multiples_of_3(10), 37);
    return clings_summary();
}
