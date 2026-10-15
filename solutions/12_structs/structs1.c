// Fix: the struct declares its fields; designated initializers set them by
// name, and `{0}` sets the first to 0 and every field left out to zero too.
/*
 * structs1
 *
 * A struct groups named fields into one type. `struct Point` below has no
 * fields yet, so the initializers that name `.x` and `.y` do not compile.
 * Give it two `int` fields, `x` and `y`. Fields left out of an initializer
 * are set to zero, which is why `{0}` zeroes a whole struct.
 * See K&R 6.1 and Beej 8.
 */

#include "clings.h"

struct Point {
    int x;
    int y;
};

int main(void) {
    struct Point p = {.x = 3, .y = 4};
    struct Point origin = {0};

    // Don't change the tests!
    CHECK_EQ_INT(p.x, 3);
    CHECK_EQ_INT(p.y, 4);
    CHECK_EQ_INT(origin.x, 0);
    return clings_summary();
}
