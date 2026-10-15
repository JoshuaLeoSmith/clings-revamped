// Fix: `&a` passes the address of `a`, so `scanf` can store into it. Passing `a`
// itself copies its (garbage) value, which scanf would treat as an address.
/*
 * io3
 *
 * This program reads two numbers with `scanf`, which is fed "3 4\n". C
 * passes arguments by value, so `scanf` cannot store into `a` and `b` as
 * written. The compiler warns about it, and warnings count as errors here.
 * See K&R 7.4 and Beej 9.4.
 */

#include <stdio.h>
#include "clings.h"

int main(void) {
    int a, b;

    // scanf returns how many values it read, so check it.
    if (scanf("%d %d", &a, &b) != 2)
        return 1;

    // Don't change the tests!
    CHECK_EQ_INT(a + b, 7);
    return clings_summary();
}
