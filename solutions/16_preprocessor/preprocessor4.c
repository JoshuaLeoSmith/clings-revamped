// Fix: `do { ... } while (0)` runs its body once and is one statement that
// needs a `;`, so the macro fits anywhere a statement does, even before an
// `else`; the braces also give `tmp` its own scope.
/*
 * preprocessor4
 *
 * `SWAP` expands to three statements, but `if (x > y) SWAP(x, y);` gives the
 * `if` room for only one, so the code after it no longer means what it
 * looks like, and the compiler rejects it. Rewrite `SWAP` so that it acts
 * as one statement, still followed by a `;`, wherever it is used.
 * See K&R 4.11.2 and Beej 19.
 */

#include <stdio.h>

#include "clings.h"

#define SWAP(a, b)     \
    do {               \
        int tmp = (a); \
        (a) = (b);     \
        (b) = tmp;     \
    } while (0)

int main(void) {
    int x = 2;
    int y = 1;
    if (x > y)
        SWAP(x, y);
    else
        printf("already ordered\n");

    // Don't change the tests!
    CHECK_EQ_INT(x, 1);
    CHECK_EQ_INT(y, 2);
    return clings_summary();
}
