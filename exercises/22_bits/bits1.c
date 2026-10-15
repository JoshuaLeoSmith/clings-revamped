/*
 * bits1
 *
 * Bit `n` of a number is worth 2 to the power n, and `1u << n` is a mask
 * with only that bit set. Use `&`, `|` and `~` to write `is_odd` (is bit 0
 * set? return 1 or 0), `set_bit` (turn bit `n` on) and `clear_bit` (turn
 * bit `n` off), leaving all the other bits of `x` as they were.
 * See K&R 2.9 and Beej 24.
 */

#include "clings.h"

int is_odd(unsigned x) {
    // TODO: odd numbers have bit 0 set.
    (void)x;
    return 0;
}

unsigned set_bit(unsigned x, int n) {
    // TODO: turn bit n on.
    (void)n;
    return x;
}

unsigned clear_bit(unsigned x, int n) {
    // TODO: turn bit n off.
    (void)n;
    return x;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(is_odd(7), 1);
    CHECK_EQ_INT(is_odd(10), 0);
    CHECK_EQ_INT(set_bit(0, 3), 8);
    CHECK_EQ_INT(set_bit(8, 3), 8);
    CHECK_EQ_INT(clear_bit(15, 0), 14);

    return clings_summary();
}
