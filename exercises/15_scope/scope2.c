/*
 * scope2
 *
 * `next_id` should hand out 1, then 2, then 3, ... But its local `id` is
 * created afresh, set to 0, every time the function is called, so every
 * call returns 1. Make `id` keep its value between calls, while still
 * being visible only inside `next_id`.
 * See K&R 4.6 and Beej 16.
 */

#include "clings.h"

int next_id(void) {
    // TODO: `id` must survive from one call to the next.
    int id = 0;
    id++;
    return id;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(next_id(), 1);
    CHECK_EQ_INT(next_id(), 2);
    CHECK_EQ_INT(next_id(), 3);
    return clings_summary();
}
