/*
 * structs2
 *
 * `.` picks a field of a struct, but `p` here is a pointer to a struct, and
 * a pointer has no fields, so the compiler rejects `p.x`. Write `(*p).x`,
 * or the shorthand everyone uses, `p->x`.
 * See K&R 6.2 and 6.4, and Beej 8.
 */

#include "clings.h"

struct Point {
    int x;
    int y;
};

void move_point(struct Point *p, int dx, int dy) {
    // TODO: reach the fields through the pointer.
    p.x += dx;
    p.y += dy;
}

int main(void) {
    struct Point p = {3, 4};
    move_point(&p, 1, 2);

    // Don't change the tests!
    CHECK_EQ_INT(p.x, 4);
    CHECK_EQ_INT(p.y, 6);
    return clings_summary();
}
