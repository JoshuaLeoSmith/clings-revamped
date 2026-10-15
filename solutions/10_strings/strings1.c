// Fix: a string literal of n letters needs n + 1 chars for its '\0'. Leaving the
// size out (`char name[] = "Hello";`) lets the compiler count it for you.
/*
 * strings1
 *
 * A C string is an array of `char` that ends with the null character '\0';
 * that is how `strlen` and `printf` know where it stops. "Hello" has five
 * letters but needs six chars. `name` has no room for the '\0', so `printf`
 * and `strlen` read past the end of the array, and AddressSanitizer reports a
 * stack-buffer-overflow. Make room for it.
 * See K&R 1.9 and Beej 7.
 */

#include <stdio.h>
#include <string.h>

#include "clings.h"

int main(void) {
    char name[] = "Hello";
    printf("%s\n", name);

    // Don't change the tests!
    CHECK_EQ_INT(strlen(name), 5);
    return clings_summary();
}
