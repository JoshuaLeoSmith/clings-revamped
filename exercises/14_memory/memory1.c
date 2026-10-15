/*
 * memory1
 *
 * `malloc` asks for memory that stays yours until you hand it back with
 * `free`; C has no garbage collector to do it for you. This program never
 * gives its array back, and the leak checker reports it when the program
 * exits. Free the array once you are done with it.
 * See K&R 7.8.5 and Beej 12.
 */

#include <stdlib.h>

#include "clings.h"

int main(void) {
    int *squares = malloc(5 * sizeof(int));
    if (squares == NULL) {
        return 1;
    }
    for (int i = 0; i < 5; i++) {
        squares[i] = i * i;
    }

    // Don't change the tests!
    CHECK_EQ_INT(squares[4], 16);

    // TODO: the array is no longer needed here; give it back.

    return clings_summary();
}
