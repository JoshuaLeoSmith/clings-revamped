/*
 * bits2
 *
 * Write `get_bit`, which returns bit `n` of `x` as 0 or 1. Then run it:
 * `1 << shift` shifts a signed `int` 1 into its sign bit, which is undefined
 * behaviour, and the sanitizer stops the program with "left shift of 1 by
 * 31 places cannot be represented in type 'int'". Make that shift unsigned.
 * See K&R 2.9 and Beej 24.
 */

#include "clings.h"

int get_bit(unsigned x, int n) {
    // TODO: shift bit n down to position 0, then keep only that bit.
    (void)x;
    (void)n;
    return 0;
}

int main(void) {
    int shift = 31;
    // TODO: a plain `1` is a signed int. How do you write an unsigned 1?
    unsigned high_bit_mask = 1 << shift;

    // Don't change the tests!
    CHECK_EQ_INT(get_bit(5, 0), 1);
    CHECK_EQ_INT(get_bit(5, 1), 0);
    CHECK_EQ_INT(get_bit(5, 2), 1);
    CHECK_EQ_INT(high_bit_mask, 2147483648LL);
    CHECK_EQ_INT(1 << 3, 8);
    CHECK_EQ_INT(64 >> 2, 16);

    return clings_summary();
}
