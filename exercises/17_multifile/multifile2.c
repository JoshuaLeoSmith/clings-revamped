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

// TODO: declare the global `call_count` that counter.c defines.

// This prototype gets `helper()` past the compiler, but counter.c keeps
// `helper` to itself, so the linker will not find it.
int helper(void);

int main(void) {
    count_event();
    count_event();
    // TODO: `helper` is static in counter.c, so it is invisible here; use
    // the public function from counter.h instead.
    helper();

    // Don't change the tests!
    CHECK_EQ_INT(get_count(), 3);
    CHECK_EQ_INT(call_count, 3);
    return clings_summary();
}
