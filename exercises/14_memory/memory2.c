/*
 * memory2
 *
 * `malloc` hands you memory with whatever bytes happen to be in it; it does
 * not zero it. The counters below start out as garbage, so counting from
 * them gives garbage. Allocate them already zeroed (or zero them yourself).
 * See K&R 7.8.5 and Beej 12.
 */

#include <stdlib.h>

#include "clings.h"

int main(void) {
    // TODO: all ten counters must start at 0.
    int *counts = malloc(10 * sizeof(int));
    if (counts == NULL) {
        return 1;
    }
    counts[3]++;
    counts[3]++;

    // Don't change the tests!
    CHECK_EQ_INT(counts[3], 2);
    CHECK_EQ_INT(counts[0], 0);
    free(counts);
    return clings_summary();
}
