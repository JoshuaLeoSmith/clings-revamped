/*
 * structs3
 *
 * Structs are passed and returned by value, like ints: the whole struct is
 * copied. So `birthday` adds a year to its own copy and `me` never changes;
 * make it take a pointer and pass it `&me`. Then implement `make_point`,
 * which returns a new `struct Point` with the given coordinates.
 * See K&R 6.2 and Beej 8.
 */

#include "clings.h"

struct Person {
    char name[32];
    int age;
};

struct Point {
    int x;
    int y;
};

// TODO: change `birthday` so it changes the caller's Person.
void birthday(struct Person p) {
    p.age++;
}

struct Point make_point(int x, int y) {
    // TODO: return a Point at (x, y).
    (void)x;
    (void)y;
    struct Point p = {0, 0};
    return p;
}

int main(void) {
    struct Person me = {"Ada", 36};
    birthday(me); // TODO: pass the address instead.

    // Don't change the tests!
    CHECK_EQ_INT(me.age, 37);
    CHECK_EQ_INT(make_point(2, 3).x, 2);
    CHECK_EQ_INT(make_point(2, 3).y, 3);
    return clings_summary();
}
