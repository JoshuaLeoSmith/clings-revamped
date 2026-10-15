/*
 * bits3
 *
 * Flags pack several yes/no options into one number: each enumerator below
 * is a different power of two, so each one owns a single bit, and
 * `PERM_READ | PERM_EXEC` means "read and execute". Write `has_flag` (1 if
 * bit `p` is set in `flags`, else 0) and `toggle_flag` (flip bit `p`).
 * See K&R 2.9 and 6.9, and Beej 24.
 */

#include "clings.h"

enum Perm { PERM_READ = 1 << 0, PERM_WRITE = 1 << 1, PERM_EXEC = 1 << 2 };

int has_flag(unsigned flags, enum Perm p) {
    // TODO: is p's bit set in flags?
    (void)flags;
    (void)p;
    return 0;
}

unsigned toggle_flag(unsigned flags, enum Perm p) {
    // TODO: turn p's bit on if it is off, and off if it is on.
    (void)p;
    return flags;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(has_flag(PERM_READ | PERM_EXEC, PERM_EXEC), 1);
    CHECK_EQ_INT(has_flag(PERM_READ, PERM_WRITE), 0);
    CHECK_EQ_INT(toggle_flag(PERM_READ, PERM_WRITE), 3);
    CHECK_EQ_INT(toggle_flag(3, PERM_WRITE), 1);

    return clings_summary();
}
