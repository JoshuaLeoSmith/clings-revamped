/*
 * memory3
 *
 * After `free(p)`, the memory `p` points to is no longer yours: `p` still
 * holds the old address, but reading through it is undefined behaviour (a
 * "use after free"). The address sanitizer catches this one. Move the
 * `free` to after the last use of `msg`.
 * See K&R 7.8.5 and Beej 12.
 */

#include <stdlib.h>
#include <string.h>

#include "clings.h"

int main(void) {
    char *msg = malloc(16);
    if (msg == NULL) {
        return 1;
    }
    strcpy(msg, "hello");
    // TODO: this is too early; `msg` is still used below.
    free(msg);

    size_t len = strlen(msg);
    // TODO: `msg` is not used after this point, so this is where `free` belongs.

    // Don't change the tests!
    CHECK_EQ_INT(len, 5);
    return clings_summary();
}
