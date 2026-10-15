/*
 * preprocessor2
 *
 * A macro with parameters pastes its arguments into the replacement text,
 * as text, not as values. So `SQUARE(2 + 1)` becomes `2 + 1 * 2 + 1`, and
 * the usual precedence rules do the rest. Add parentheses to `SQUARE` until
 * every check passes.
 * See K&R 4.11.2 and Beej 19.
 */

#include "clings.h"

// TODO: make SQUARE work whatever expression it is given and wherever it is used.
#define SQUARE(x) x * x

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(SQUARE(3), 9);
    CHECK_EQ_INT(SQUARE(2 + 1), 9);
    CHECK_EQ_INT(100 / SQUARE(5), 4);
    return clings_summary();
}
