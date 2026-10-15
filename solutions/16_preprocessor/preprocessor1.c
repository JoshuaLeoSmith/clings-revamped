// Fix: the replacement text is everything after the name, `;` included, so
// `int items[MAX_ITEMS];` became `int items[100;];`. Macros take no `;`.
/*
 * preprocessor1
 *
 * `#define` makes the preprocessor replace a name with the rest of the line,
 * as plain text, before the compiler sees the code; it is not a statement.
 * The compiler error points into the line that uses `MAX_ITEMS`: read the
 * column it gives carefully, and ask what that line looks like once the
 * name has been replaced.
 * See K&R 4.11.2 and Beej 19.
 */

#include "clings.h"

#define MAX_ITEMS 100

int main(void) {
    int items[MAX_ITEMS];

    // Don't change the tests!
    CHECK_EQ_INT(sizeof(items) / sizeof(items[0]), 100);
    return clings_summary();
}
