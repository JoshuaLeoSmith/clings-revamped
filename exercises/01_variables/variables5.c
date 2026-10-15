/*
 * variables5
 *
 * In C, single and double quotes mean different things. A `char` is a
 * single small integer holding one character, and a string is something else.
 * `letter` should hold the character a.
 * See K&R 2.2-2.3 and Beej 3.1.
 */

#include "clings.h"

int main(void) {
    // TODO: this does not compile. Look closely at the quotes.
    char letter = "a";

    // Don't change the tests!
    CHECK_EQ_CHAR(letter, 'a');
    return clings_summary();
}
