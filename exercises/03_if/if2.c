/*
 * if2
 *
 * `is_five` should return 1 when its argument is 5 and 0 otherwise, but it
 * says yes to everything. The compiler has a warning about it, and warnings
 * count as errors in this exercise.
 * See K&R 3.2 and Beej 3.3.1.
 */

#include "clings.h"

int is_five(int x) {
    // TODO: this condition is not asking the question you think it is.
    if (x = 5)
        return 1;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(is_five(5), 1);
    CHECK_EQ_INT(is_five(3), 0);
    return clings_summary();
}
