/*
 * types1
 *
 * `sizeof` gives the size of a type in bytes, as a `size_t`: an unsigned type
 * that may be wider than `int`. `printf` trusts its format string, so `%d`
 * for a `size_t` is a bug the compiler warns about. Fix the format, then
 * compute how many bytes ten `double`s take.
 * See K&R 2.2 and Beej 14.
 */

#include <stdio.h>

#include "clings.h"

int main(void) {
    // TODO: use the right conversion for a size_t.
    printf("An int is %d bytes\n", sizeof(int));

    // TODO: compute the size of ten doubles with sizeof.
    size_t bytes_for_doubles = 10;

    // Don't change the tests!
    CHECK_EQ_INT(bytes_for_doubles, 10 * sizeof(double));
    return clings_summary();
}
