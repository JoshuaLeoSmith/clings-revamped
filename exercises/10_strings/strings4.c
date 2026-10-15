/*
 * strings4
 *
 * `==` on two strings compares where they are stored, not the chars in
 * them, so it is almost never what you want. The compiler warns, and
 * warnings count as errors here. `strcmp(a, b)` from <string.h> compares the
 * contents and returns 0 when they are equal.
 * See K&R 7.8.1 and Beej 7.
 */

#include <string.h>

#include "clings.h"

int is_yes(const char answer[]) {
    // TODO: compare the contents of the strings.
    return answer == "yes";
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(is_yes("yes"), 1);
    CHECK_EQ_INT(is_yes("no"), 0);
    return clings_summary();
}
