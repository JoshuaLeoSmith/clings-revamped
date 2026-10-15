/*
 * text3
 *
 * `sprintf` writes as many chars as the result needs, however small the
 * buffer is: here it overflows `line` and the address sanitizer stops it.
 * Make `line` big enough AND switch to `snprintf(line, sizeof line, ...)`,
 * which never writes past the size it is given (a buffer that is too small
 * then truncates the text, and the check still fails). Then write
 * `split_words`: store up to `max` space-separated words of `s` in `out`
 * using `strtok`, and return how many. See K&R 5.5 and Beej 15.1.
 */

#include <stdio.h>
#include <string.h>

#include "clings.h"

int split_words(char *s, char *out[], int max) {
    // TODO: use strtok(s, " ") for the first word, strtok(NULL, " ") after.
    (void)s;
    (void)out;
    (void)max;
    return 0;
}

int main(void) {
    // TODO: this buffer is too small, and `sprintf` does not care.
    char line[16];
    sprintf(line, "Player %s scored %d points", "Alexandra", 1200);
    char text[] = "one two  three";
    char *out[8] = {NULL};
    int count = split_words(text, out, 8);

    // Don't change the tests!
    CHECK_EQ_STR(line, "Player Alexandra scored 1200 points");
    CHECK_EQ_INT(count, 3);
    CHECK_EQ_STR(out[2], "three");
    return clings_summary();
}
