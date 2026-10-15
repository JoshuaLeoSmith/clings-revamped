// Fix: `sizeof` yields a `size_t`, whose printf conversion is `%zu`; `%d`
// expects an `int`, which may be a different size. sizeof(double) * 10 counts bytes.
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
    printf("An int is %zu bytes\n", sizeof(int));

    size_t bytes_for_doubles = 10 * sizeof(double);

    // Don't change the tests!
    CHECK_EQ_INT(bytes_for_doubles, 10 * sizeof(double));
    return clings_summary();
}
