/*
 * arrays2
 *
 * In `main`, `sizeof(nums) / sizeof(nums[0])` counts the elements. Inside
 * `sum` the same trick fails: an array parameter is really a pointer to the
 * first element, so `sizeof(values)` is the size of a pointer. The compiler
 * warns, and warnings count as errors here. Use the length you are given.
 * See K&R 5.7 and Beej 6.
 */

#include <stddef.h>

#include "clings.h"

int sum(int values[], size_t n) {
    int total = 0;
    // TODO: loop over the length that was passed in.
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        total += values[i];
    }
    return total;
}

int main(void) {
    int nums[] = {1, 2, 3, 4, 5};
    size_t len = sizeof(nums) / sizeof(nums[0]);

    // Don't change the tests!
    CHECK_EQ_INT(len, 5);
    CHECK_EQ_INT(sum(nums, len), 15);
    return clings_summary();
}
