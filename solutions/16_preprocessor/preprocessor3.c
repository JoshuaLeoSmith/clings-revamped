// Fix: a macro must be defined before the lines that use it, the `#if` included;
// in an `#if`, a name that is not a macro silently counts as 0.
/*
 * preprocessor3
 *
 * `#ifndef` / `#endif` gives `GREETING` a default unless something defined
 * it first, and `#if` / `#error` stops the build if `MAX_USERS` is too
 * large. But `MAX_USERS` is never defined, so the compiler fails at the
 * array that uses it. Define it as 50 where the TODO is.
 * See K&R 4.11.3 and Beej 19.
 */

#include "clings.h"

#define MAX_USERS 50

#ifndef GREETING
#define GREETING "Hello"
#endif

#if MAX_USERS > 100
#error "MAX_USERS is too large"
#endif

int main(void) {
    int users[MAX_USERS];

    // Don't change the tests!
    CHECK_EQ_STR(GREETING, "Hello");
    CHECK_EQ_INT(MAX_USERS, 50);
    CHECK_EQ_INT(sizeof(users) / sizeof(users[0]), 50);
    return clings_summary();
}
