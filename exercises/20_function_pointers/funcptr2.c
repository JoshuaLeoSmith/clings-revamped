/*
 * funcptr2
 *
 * A `typedef` gives a function pointer type a short name: `IntFn` is "a
 * pointer to a function taking an int and returning an int". Write `apply`,
 * which calls `f` on each of the `n` values and stores each result back in
 * place, so the same `apply` can double, negate, or do anything else.
 * See K&R 5.11 and Beej 23.7.
 */

#include "clings.h"

typedef int (*IntFn)(int);

int double_it(int x) {
    return 2 * x;
}

int negate(int x) {
    return -x;
}

void apply(int *values, int n, IntFn f) {
    // TODO: replace each value with the result of calling `f` on it.
    (void)values;
    (void)n;
    (void)f;
}

int main(void) {
    // Don't change the tests!
    int nums[] = {1, 2, 3};
    apply(nums, 3, double_it);
    CHECK_EQ_INT(nums[0], 2);
    CHECK_EQ_INT(nums[1], 4);
    CHECK_EQ_INT(nums[2], 6);
    apply(nums, 3, negate);
    CHECK_EQ_INT(nums[0], -2);
    CHECK_EQ_INT(nums[1], -4);
    CHECK_EQ_INT(nums[2], -6);

    return clings_summary();
}
