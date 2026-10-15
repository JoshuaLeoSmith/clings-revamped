// Fix: `/` binds tighter than `+`, and `==` tighter than `&`, so both need
// parentheses to group the way they read: (a + b) / 2 and (x & 1) == 0.
/*
 * operators2
 *
 * Both functions here are grouped wrongly. Precedence decides which operator
 * applies first when there are no parentheses, and C's table has surprises:
 * `==` binds tighter than `&`. The compiler warns about one of these bugs,
 * and warnings count as errors here; the checks catch the other.
 * See K&R 2.12 and Beej 3.2.
 */

#include "clings.h"

int average(int a, int b) {
    return (a + b) / 2;
}

int is_even(int x) {
    return (x & 1) == 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(average(3, 5), 4);
    CHECK_EQ_INT(average(10, 20), 15);
    CHECK_EQ_INT(is_even(4), 1);
    CHECK_EQ_INT(is_even(7), 0);
    return clings_summary();
}
