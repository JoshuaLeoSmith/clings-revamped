/*
 * variables1
 *
 * In C every variable must be declared, with its type, before it is used.
 * This program should store 5 in `x` and print it, but it does not compile.
 * See K&R 1.2 and 2.1, and Beej 3.1.
 */

#include <stdio.h>

int main(void) {
    // TODO: `x` has never been declared. Fix this line.
    x = 5;

    printf("x has the value %d\n", x);
    return 0;
}
