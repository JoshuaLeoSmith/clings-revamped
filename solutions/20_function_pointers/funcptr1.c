// Fix: `int *op(int, int)` declares a function returning `int *`. The
// parentheses in `int (*op)(int, int)` bind `*` to `op` first: a pointer to a function.
/*
 * funcptr1
 *
 * A variable can hold the address of a function and call it later. `op`
 * is meant to be such a variable, but the way it is declared makes it
 * something else entirely, so the program does not compile. Read the
 * compiler error and fix the declaration of `op`.
 * See K&R 5.11 and Beej 23.7.
 */

#include "clings.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main(void) {
    int (*op)(int, int) = add;

    // Don't change the tests!
    CHECK_EQ_INT(op(2, 3), 5);
    op = multiply;
    CHECK_EQ_INT(op(2, 3), 6);

    return clings_summary();
}
