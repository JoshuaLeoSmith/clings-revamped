/*
 * structs4
 *
 * A struct can contain other structs, and arrays can hold structs. Implement
 * `area`, which gives the area of a rectangle from its top-left and
 * bottom-right corners (y grows downwards, as on a screen), and
 * `total_age`, which adds up the ages of `n` people.
 * See K&R 6.3 and Beej 8.
 */

#include "clings.h"

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point top_left, bottom_right;
};

struct Person {
    char name[32];
    int age;
};

int area(struct Rect r) {
    // TODO: width times height.
    (void)r;
    return 0;
}

int total_age(const struct Person people[], int n) {
    // TODO: add up people[0].age to people[n - 1].age.
    (void)people;
    (void)n;
    return 0;
}

int main(void) {
    struct Rect r = {{0, 0}, {4, 3}};
    struct Person people[] = {{"Ann", 30}, {"Bob", 40}, {"Cy", 50}};

    // Don't change the tests!
    CHECK_EQ_INT(area(r), 12);
    CHECK_EQ_INT(total_age(people, 3), 120);
    return clings_summary();
}
