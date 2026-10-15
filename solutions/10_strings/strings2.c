// Fix: a string's length is not stored anywhere; it is found by scanning for
// the '\0' that ends it, which is all `strlen` does.
/*
 * strings2
 *
 * `strlen` counts the chars before the terminating '\0'. Implement your own
 * version: walk through the array until you reach '\0' and return how many
 * chars came before it.
 * See K&R 1.9 and Beej 7.
 */

#include <stddef.h>

#include "clings.h"

size_t my_strlen(const char s[]) {
    size_t n = 0;
    while (s[n] != '\0') {
        n++;
    }
    return n;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(my_strlen("hello"), 5);
    CHECK_EQ_INT(my_strlen(""), 0);
    CHECK_EQ_INT(my_strlen("a b"), 3);
    return clings_summary();
}
