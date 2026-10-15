// Fix: `int squares[6];` reserves six ints, squares[0] to squares[5]. The
// size is part of the array's type, so sizeof(squares) is 6 * sizeof(int).
/*
 * arrays1
 *
 * An array holds a fixed number of elements of one type, and the size is
 * part of its declaration: `int a[3]` is three `int`s, `a[0]` to `a[2]`.
 * Declare the array `squares` so the loop can fill it with 0, 1, 4, ..., 25.
 * See K&R 1.6 and Beej 6.
 */

#include "clings.h"

int main(void) {
    int squares[6];

    for (int i = 0; i < 6; i++) {
        squares[i] = i * i;
    }

    // Don't change the tests!
    CHECK_EQ_INT(squares[0], 0);
    CHECK_EQ_INT(squares[5], 25);
    CHECK_EQ_INT(sizeof(squares) / sizeof(squares[0]), 6);
    return clings_summary();
}
