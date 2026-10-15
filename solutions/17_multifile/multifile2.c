// Fix: `extern` declares a global defined in another file without creating
// a new one; a file-scope `static` function is visible only in its own file.
/*
 * multifile2
 *
 * counter.c (compiled alongside this file) keeps a global `call_count`
 * and a `static` function `helper`. Two problems: `call_count` needs a
 * declaration here, and `helper` is private to counter.c. Fix the first
 * and you get a *linker* error ("undefined reference to `helper'"), not a
 * compiler error: each file compiled, but the pieces did not join up.
 * See K&R 4.5 and Beej 17.
 */

#include "clings.h"
#include "counter.h"

extern int call_count;

int main(void) {
    count_event();
    count_event();
    count_event();

    // Don't change the tests!
    CHECK_EQ_INT(get_count(), 3);
    CHECK_EQ_INT(call_count, 3);
    return clings_summary();
}
