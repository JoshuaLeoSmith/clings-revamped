// Fix: `x & mask` keeps only the masked bit, `x | mask` turns it on, and
// `x & ~mask` turns it off, because `~mask` has every bit set except that one.
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
    return (x & 1u) != 0;
}

unsigned set_bit(unsigned x, int n) {
    return x | (1u << n);
}

unsigned clear_bit(unsigned x, int n) {
    return x & ~(1u << n);
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
