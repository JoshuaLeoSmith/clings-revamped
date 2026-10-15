/*
 * operators1
 *
 * `i++` and `++i` both add one to `i`, but they give different values:
 * `i++` gives the value from before the increment, `++i` the value after.
 * The same goes for `k--` and `--k`. Make `j` and `m` get the new values.
 * See K&R 2.8 and Beej 3.2.
 */

#include "clings.h"

int main(void) {
    // TODO: fix both lines so that j and m hold the updated values.
    int i = 5;
    int j = i++;
    int k = 10;
    int m = k--;

    // Don't change the tests!
    CHECK_EQ_INT(j, 6);
    CHECK_EQ_INT(i, 6);
    CHECK_EQ_INT(m, 9);
    CHECK_EQ_INT(k, 9);
    return clings_summary();
}
