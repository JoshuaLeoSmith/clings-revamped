/*
 * loops2
 *
 * `count_digits` should return how many decimal digits `n` has, but its
 * `while` loop never ends. When clings reports "killed after 5 seconds, is
 * there an infinite loop?", it stopped your program because it was still
 * running: the loop's condition never became false. Make the loop progress.
 * See K&R 3.5 and Beej 3.3.2.
 */

#include "clings.h"

int count_digits(int n) {
    long count = 0;
    while (n > 0) {
        count++;
        // TODO: nothing in this loop changes `n`, so `n > 0` stays true forever.
    }
    return count;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(count_digits(12345), 5);
    CHECK_EQ_INT(count_digits(7), 1);
    return clings_summary();
}
