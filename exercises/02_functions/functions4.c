/*
 * functions4
 *
 * `increment` is supposed to add one to the caller's number, but `a` is
 * still 5 afterwards. Change `increment` and how `main` calls it so that
 * `a` becomes 6. (You will meet pointers later; you don't need them here.)
 * See K&R 1.8 and Beej 4.
 */

#include "clings.h"

// TODO: this changes something, but not `a`.
void increment(int x) {
    x++;
}

int main(void) {
    int a = 5;
    increment(a);

    // Don't change the tests!
    CHECK_EQ_INT(a, 6);
    return clings_summary();
}
