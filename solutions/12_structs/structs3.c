// Fix: a struct parameter is a copy, so birthday takes a pointer and is
// passed &me. Returning a struct by value copies it out to the caller.
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

void birthday(struct Person *p) {
    p->age++;
}

struct Point make_point(int x, int y) {
    struct Point p = {x, y};
    return p;
}

int main(void) {
    struct Person me = {"Ada", 36};
    birthday(&me);

    // Don't change the tests!
    CHECK_EQ_INT(me.age, 37);
    CHECK_EQ_INT(make_point(2, 3).x, 2);
    CHECK_EQ_INT(make_point(2, 3).y, 3);
    return clings_summary();
}
