// Fix: C passes arguments by value, so `x` was a copy of `a` and incrementing it
// changed only the copy. Returning the new value lets the caller store it.
/*
 * functions4
 *
 * `increment` is supposed to add one to the caller's number, but `a` is
 * still 5 afterwards. Change `increment` and how `main` calls it so that
 * `a` becomes 6. (You will meet pointers later; you don't need them here.)
 * See K&R 1.8 and Beej 4.
 */

#include "clings.h"

int increment(int x) {
    return x + 1;
}

int main(void) {
    int a = 5;
    a = increment(a);

    // Don't change the tests!
    CHECK_EQ_INT(a, 6);
    return clings_summary();
}
