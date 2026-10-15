// Fix: *a and *b are the caller's `const char *` variables themselves, so
// swapping them makes x and y point to each other's strings; no chars are copied.
/*
 * pointers5
 *
 * A pointer can point to another pointer. `x` and `y` are `const char *`
 * values that point to strings; to swap which string each one points to,
 * `swap_strings` needs their addresses, `const char **`. Implement it so
 * that afterwards `x` points to "right" and `y` to "left".
 * See K&R 5.6 and Beej 23.
 */

#include "clings.h"

void swap_strings(const char **a, const char **b) {
    const char *tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(void) {
    const char *x = "left", *y = "right";
    swap_strings(&x, &y);

    // Don't change the tests!
    CHECK_EQ_STR(x, "right");
    CHECK_EQ_STR(y, "left");
    return clings_summary();
}
