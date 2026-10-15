// Fix: `qsort` works on any type, so it passes `const void *`; convert them and
// compare `*x` and `*y`. `*x - *y` could overflow an int; `(a > b) - (a < b)` cannot.
/*
 * funcptr3
 *
 * `qsort` sorts any array: you give it the element count and size plus a
 * comparison function, which it calls with pointers to two elements. The
 * comparator must return a negative number, zero, or a positive number when
 * the first element is less than, equal to, or greater than the second.
 * This one compares the wrong thing, so the array does not get sorted.
 * See K&R 5.11 and Beej 23.7.
 */

#include <stdlib.h>

#include "clings.h"

int compare_ints(const void *a, const void *b) {
    const int *x = a;
    const int *y = b;
    return (*x > *y) - (*x < *y);
}

int main(void) {
    int values[] = {5, 2, 9, 1};
    qsort(values, 4, sizeof values[0], compare_ints);

    // Don't change the tests!
    CHECK_EQ_INT(values[0], 1);
    CHECK_EQ_INT(values[1], 2);
    CHECK_EQ_INT(values[2], 5);
    CHECK_EQ_INT(values[3], 9);

    return clings_summary();
}
