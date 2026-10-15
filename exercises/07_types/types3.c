/*
 * types3
 *
 * A `char` is a small integer holding a character code, so you can add to it
 * and compare it like a number: 'a' + 1 is 'b'. Implement `next_letter`
 * (where 'z' wraps around to 'a') and `to_upper_char` (which leaves anything
 * that is not a lowercase letter alone) using arithmetic, without a library.
 * See K&R 2.2 and 2.7, and Beej 14.
 */

#include "clings.h"

char next_letter(char c) {
    // TODO: return the letter after `c`.
    return c;
}

char to_upper_char(char c) {
    // TODO: turn a lowercase letter into its uppercase form.
    return c;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_CHAR(next_letter('a'), 'b');
    CHECK_EQ_CHAR(next_letter('z'), 'a');
    CHECK_EQ_CHAR(to_upper_char('q'), 'Q');
    CHECK_EQ_CHAR(to_upper_char('Q'), 'Q');
    CHECK_EQ_CHAR(to_upper_char('3'), '3');
    return clings_summary();
}
