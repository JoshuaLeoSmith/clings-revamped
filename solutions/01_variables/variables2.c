// Fix: initialize `x` where it is declared. Reading an uninitialized local is
// undefined behaviour in C; there is no default value like 0 or null.
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
    int x = 10;

    printf("x has the value %d\n", x);

    // Don't change the tests!
    CHECK_EQ_INT(x, 10);
    return clings_summary();
}
