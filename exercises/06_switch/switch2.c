/*
 * switch2
 *
 * `is_vowel` should return 1 for a vowel (a, e, i, o, u, in either case) and
 * 0 for anything else. Write it with a `switch`: several `case` labels can
 * share one body, and `default` handles everything not listed.
 * See K&R 3.4 and Beej 3.3.5.
 */

#include "clings.h"

int is_vowel(char c) {
    // TODO: write a switch on `c`.
    (void)c;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(is_vowel('a'), 1);
    CHECK_EQ_INT(is_vowel('E'), 1);
    CHECK_EQ_INT(is_vowel('x'), 0);
    return clings_summary();
}
