// Fix: compare the pointer with NULL before dereferencing it. Any pointer that
// may be NULL must be checked; C does not do it for you.
/*
 * pointers3
 *
 * NULL is a pointer that points nowhere, and following it (`*p` when `p` is
 * NULL) is undefined behaviour; on most systems the program crashes. Here
 * the sanitizer stops the program with "load of null pointer". `first_or`
 * returns the int `p` points to, or `fallback` when `p` is NULL.
 * See K&R 5.4 and Beej 5.
 */

#include <stddef.h>

#include "clings.h"

int first_or(const int *p, int fallback) {
    if (p == NULL) {
        return fallback;
    }
    return *p;
}

int main(void) {
    int x = 42;

    // Don't change the tests!
    CHECK_EQ_INT(first_or(&x, -1), 42);
    CHECK_EQ_INT(first_or(NULL, -1), -1);
    return clings_summary();
}
