// Fix: a name is visible from its declaration to the end of the enclosing
// block, so `answer` is declared before the `if` and only assigned inside it.
/*
 * scope1
 *
 * A variable declared inside a block `{ ... }` exists only until that
 * block's closing brace. `answer` is declared inside the `if`, so the
 * compiler does not know it at the check below. Declare it where the code
 * that uses it can see it.
 * See K&R 4.4 and 4.8, and Beej 13.
 */

#include "clings.h"

int main(void) {
    int limit = 10;
    int answer = 0;
    if (limit > 5) {
        answer = 42;
    }

    // Don't change the tests!
    CHECK_EQ_INT(answer, 42);
    return clings_summary();
}
