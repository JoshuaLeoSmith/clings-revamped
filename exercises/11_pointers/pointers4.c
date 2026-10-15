/*
 * pointers4
 *
 * Adding n to a pointer into an array moves it n elements along, so
 * `*(nums + 2)` is the same element as `nums[2]`. A range can be given as
 * two pointers: `begin` points to the first element and `end` just past the
 * last one. Implement `sum_range`, which adds up the ints from `begin` up to
 * but not including `end`, by moving a pointer from one to the other.
 * See K&R 5.3 and 5.4, and Beej 11.
 */

#include "clings.h"

int sum_range(const int *begin, const int *end) {
    // TODO: walk a pointer from `begin` to `end`, adding what it points to.
    (void)begin;
    (void)end;
    return 0;
}

int main(void) {
    int nums[] = {1, 2, 3, 4, 5};

    // Don't change the tests!
    CHECK_EQ_INT(sum_range(nums, nums + 5), 15);
    CHECK_EQ_INT(sum_range(nums + 1, nums + 3), 5);
    CHECK_EQ_INT(sum_range(nums, nums), 0);
    CHECK_EQ_INT(*(nums + 2), 3);
    return clings_summary();
}
