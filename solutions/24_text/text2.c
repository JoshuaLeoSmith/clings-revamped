// Fix: the digit chars '0' to '9' are consecutive in every C character set,
// so `c - '0'` is the digit's value; each new digit shifts the total one
// decimal place left (`10 * n`) before it is added.
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
    while (isspace((unsigned char)*s)) {
        s++;
    }
    int sign = 1;
    if (*s == '+' || *s == '-') {
        if (*s == '-') {
            sign = -1;
        }
        s++;
    }
    int n = 0;
    for (; isdigit((unsigned char)*s); s++) {
        n = 10 * n + (*s - '0');
    }
    return sign * n;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(my_atoi("42"), 42);
    CHECK_EQ_INT(my_atoi("  -17"), -17);
    CHECK_EQ_INT(my_atoi("+8xyz"), 8);
    CHECK_EQ_INT(my_atoi("abc"), 0);
    return clings_summary();
}
