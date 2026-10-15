// Fix: a declaration names the variable's type (`int x`), so the compiler knows
// how much storage it needs and what operations make sense on it.
/*
 * variables1
 *
 * In C every variable must be declared, with its type, before it is used.
 * This program should store 5 in `x` and print it, but it does not compile.
 * See K&R 1.2 and 2.1, and Beej 3.1.
 */

#include <stdio.h>

int main(void) {
    int x = 5;

    printf("x has the value %d\n", x);
    return 0;
}
