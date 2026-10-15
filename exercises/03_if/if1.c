/*
 * if1
 *
 * `bigger` should return the larger of its two arguments. Write its body
 * with `if` and `else`; don't use a library function.
 * See K&R 3.2 and Beej 3.3.1.
 */

#include "clings.h"

int bigger(int a, int b) {
    // TODO: complete this function.
    (void)a;
    (void)b;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(bigger(10, 20), 20);
    CHECK_EQ_INT(bigger(30, 20), 30);
    CHECK_EQ_INT(bigger(-5, -10), -5);
    return clings_summary();
}
