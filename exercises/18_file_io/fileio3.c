/*
 * fileio3
 *
 * `fopen` returns NULL when the file does not exist, and passing that NULL
 * on to `fscanf` or `fclose` crashes the program. `sum_numbers` forgets to
 * check, so the test with a missing file brings everything down. Return -1
 * when the file cannot be opened.
 * See K&R 7.5 and 7.6, and Beej 9.
 */

#include <stdio.h>

#include "clings.h"

long sum_numbers(const char *path) {
    FILE *f = fopen(path, "r");
    // TODO: what if `f` is NULL?
    long sum = 0;
    long x;
    while (fscanf(f, "%ld", &x) == 1) {
        sum += x;
    }
    fclose(f);
    return sum;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(sum_numbers("fileio3.txt"), 100);
    CHECK_EQ_INT(sum_numbers("nope.txt"), -1);
    return clings_summary();
}
