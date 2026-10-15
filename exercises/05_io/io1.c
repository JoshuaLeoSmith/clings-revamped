/*
 * io1
 *
 * `count_chars` should read standard input to the end and return how many
 * characters it read. clings feeds this program the text "hello\nworld\n".
 * Use `getchar()`, which returns the next character, or `EOF` at the end.
 * See K&R 1.5.1-1.5.2 and Beej 9.4.
 */

#include <stdio.h>
#include "clings.h"

long count_chars(void) {
    // TODO: read with getchar() until it returns EOF, counting as you go.
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(count_chars(), 12);
    return clings_summary();
}
