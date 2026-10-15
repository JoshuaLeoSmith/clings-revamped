/*
 * io2
 *
 * Count the lines and the words on standard input, like the Unix `wc`.
 * A word is a run of characters without a space, tab, or newline. clings
 * feeds this program four lines of text, one of them empty. Remember whether
 * you are inside a word, and count a word when you enter one.
 * See K&R 1.5.3-1.5.4 and Beej 9.4.
 */

#include <stdio.h>
#include "clings.h"

int main(void) {
    long lines = 0, words = 0;
    int c;

    // TODO: read every character with getchar(), updating `lines` and
    // `words`. Remove the `(void)c;` line once you use `c`.
    (void)c;

    // Don't change the tests!
    CHECK_EQ_INT(lines, 4);
    CHECK_EQ_INT(words, 9);
    return clings_summary();
}
