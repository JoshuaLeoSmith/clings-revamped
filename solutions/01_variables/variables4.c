// Fix: a `const` variable gets its value once, in its initializer, and can never
// be assigned to afterwards. So give it the right value where it is declared.
/*
 * variables4
 *
 * `const` tells the compiler a variable must never change after it is
 * initialized, and the compiler enforces it. `limit` should end up as 20.
 * Make that happen while keeping `limit` a `const int`.
 * See K&R 2.4 and Beej 3.1.
 */

#include "clings.h"

int main(void) {
    const int limit = 20;

    // Don't change the tests!
    CHECK_EQ_INT(limit, 20);
    return clings_summary();
}
