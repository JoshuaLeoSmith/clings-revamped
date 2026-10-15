// Fix: `&x` is the address of x, which is what an `int *` holds. Then `*p`
// names x itself, so `*p = 20` stores 20 in x.
/*
 * pointers1
 *
 * A pointer holds the address of another variable. `&x` is the address of
 * `x`, and `*p` is the variable that `p` points to. `p` must point to `x`,
 * so that writing through `p` changes `x`; the compiler rejects the
 * declaration below because it gives `p` the value of `x`, not its address.
 * See K&R 5.1 and Beej 5.
 */

#include "clings.h"

int main(void) {
    int x = 10;
    int *p = &x;
    *p = 20;

    // Don't change the tests!
    CHECK_EQ_INT(x, 20);
    CHECK(p == &x);
    return clings_summary();
}
