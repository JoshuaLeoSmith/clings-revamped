// Fix: the destination must hold every char of the source plus its '\0';
// strcpy cannot check that, so the caller has to size the buffer.
/*
 * strings3
 *
 * `strcpy(dst, src)` copies `src` and its '\0' into `dst`, and it never
 * checks whether they fit. "Hello, world!" needs 14 chars, so copying it
 * into `buf` writes past the end of the array. Make `buf` big enough.
 * See K&R 7.8.1 and Beej 7.
 */

#include <string.h>

#include "clings.h"

int main(void) {
    char buf[32];
    strcpy(buf, "Hello, world!");

    // Don't change the tests!
    CHECK_EQ_STR(buf, "Hello, world!");
    return clings_summary();
}
