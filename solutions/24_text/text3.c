// Fix: `snprintf` is bounded by `sizeof line`, so it cannot overflow; the
// buffer must still fit the 35 chars plus '\0'. `strtok` writes '\0' into its
// input and keeps its position in hidden static state, so it needs a writable
// string and cannot split two strings at once (it is not reentrant).
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
    int n = 0;
    while (n < max) {
        char *word = strtok(n == 0 ? s : NULL, " ");
        if (word == NULL) {
            break;
        }
        out[n] = word;
        n++;
    }
    return n;
}

int main(void) {
    char line[64];
    snprintf(line, sizeof line, "Player %s scored %d points", "Alexandra", 1200);
    char text[] = "one two  three";
    char *out[8] = {NULL};
    int count = split_words(text, out, 8);

    // Don't change the tests!
    CHECK_EQ_STR(line, "Player Alexandra scored 1200 points");
    CHECK_EQ_INT(count, 3);
    CHECK_EQ_STR(out[2], "three");
    return clings_summary();
}
