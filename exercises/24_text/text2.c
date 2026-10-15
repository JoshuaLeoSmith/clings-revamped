/*
 * text2
 *
 * Write `my_atoi`, which turns the digits at the start of a string into an
 * int, like the standard `atoi`: skip leading whitespace, accept one
 * optional '+' or '-', then read digits until the first non-digit. A
 * string with no digits gives 0. The digit char `c` is worth `c - '0'`.
 * See K&R 2.7 and Beej 15.1.
 */

#include <ctype.h>

#include "clings.h"

int my_atoi(const char *s) {
    // TODO: skip spaces, read the sign, then the digits.
    (void)s;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(my_atoi("42"), 42);
    CHECK_EQ_INT(my_atoi("  -17"), -17);
    CHECK_EQ_INT(my_atoi("+8xyz"), 8);
    CHECK_EQ_INT(my_atoi("abc"), 0);
    return clings_summary();
}
