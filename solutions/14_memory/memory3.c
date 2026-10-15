// Fix: free memory only after its last use. A freed pointer is "dangling": it
// still holds an address, but nothing valid lives there any more.
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

    size_t len = strlen(msg);
    free(msg);

    // Don't change the tests!
    CHECK_EQ_INT(len, 5);
    return clings_summary();
}
