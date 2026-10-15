// Fix: the struct has no tag, only the typedef name, so it is declared as
// `Vec2 v`; `struct Vec2` would be a different, never-defined type.
/*
 * typedef1
 *
 * `typedef` gives a type a new name. `Vec2` below names a struct that has
 * no tag of its own, so there is no `struct Vec2`: the compiler sees an
 * unknown, incomplete type and cannot make `v`. Use the typedef name, as
 * `length` does. `byte` is another name for `unsigned char`.
 * See K&R 6.7 and Beej 10.
 */

#include <math.h>

#include "clings.h"

typedef struct {
    double x, y;
} Vec2;

typedef unsigned char byte;

double length(Vec2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

int main(void) {
    Vec2 v = {3.0, 4.0};
    byte b = 255;

    // Don't change the tests!
    CHECK_EQ_DBL(length(v), 5.0);
    CHECK_EQ_INT(b, 255);
    CHECK_EQ_INT(sizeof(byte), 1);
    return clings_summary();
}
