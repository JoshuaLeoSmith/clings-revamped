/*
 * scope3
 *
 * `bump` is meant to add one to the global `counter`. But it declares a
 * local variable with the same name, which hides ("shadows") the global
 * inside the function, so `counter++` changes the local, and an
 * uninitialized one at that. This exercise treats warnings as errors.
 * See K&R 4.3 and 4.4, and Beej 13.
 */

#include "clings.h"

int counter = 0;

void bump(void) {
    // TODO: `bump` should change the global `counter`.
    int counter;
    counter++;
}

int main(void) {
    bump();
    bump();

    // Don't change the tests!
    CHECK_EQ_INT(counter, 2);
    return clings_summary();
}
