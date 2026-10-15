/*
 * pointers2
 *
 * C passes arguments by value, so a function gets copies and cannot change
 * the caller's variables (remember functions4). Passing their addresses
 * instead lets the function reach the originals through `*a` and `*b`.
 * Implement `swap` so it exchanges the two ints the pointers point to.
 * See K&R 5.2 and Beej 5.
 */

#include "clings.h"

void swap(int *a, int *b) {
    // TODO: exchange *a and *b.
    (void)a;
    (void)b;
}

int main(void) {
    int a = 1, b = 2;
    swap(&a, &b);

    // Don't change the tests!
    CHECK_EQ_INT(a, 2);
    CHECK_EQ_INT(b, 1);
    return clings_summary();
}
