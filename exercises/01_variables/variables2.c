/*
 * variables2
 *
 * A local variable in C is not set to zero for you: until you assign to it,
 * it holds whatever happened to be in that memory. This program should print
 * `x` with the value 10. Warnings count as errors in this exercise.
 * See K&R 2.4 and Beej 3.1.
 */

#include <stdio.h>
#include "clings.h"

int main(void) {
    // TODO: `x` is read before it has been given a value.
    int x;

    printf("x has the value %d\n", x);

    // Don't change the tests!
    CHECK_EQ_INT(x, 10);
    return clings_summary();
}
