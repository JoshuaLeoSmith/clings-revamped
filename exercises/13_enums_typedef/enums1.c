/*
 * enums1
 *
 * An enum names a set of int constants. The tests use `BLUE`, which
 * `enum Color` does not have yet, so first the file does not compile: add
 * it. Then the compiler warns that the switch in `color_name` does not
 * handle `BLUE` (with no `default:`, gcc checks every enumerator has a
 * `case`), and warnings count as errors here: add the case.
 * See K&R 2.3 and Beej 22.
 */

#include "clings.h"

// TODO: add BLUE.
enum Color { RED, GREEN };

const char *color_name(enum Color c) {
    switch (c) {
    case RED:
        return "red";
    case GREEN:
        return "green";
    }
    return "unknown";
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_STR(color_name(BLUE), "blue");
    CHECK_EQ_STR(color_name(RED), "red");
    return clings_summary();
}
