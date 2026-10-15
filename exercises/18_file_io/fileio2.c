/*
 * fileio2
 *
 * `fprintf(f, ...)` works like `printf` but writes to a file opened with
 * mode "w"; `fscanf(f, "%d", &x)` reads a number back and returns how many
 * values it read (1), or less at the end of the file. Write the numbers
 * 1..n one per line, then read them back and add them up. Return -1 from
 * either function if the file cannot be opened.
 * See K&R 7.5 and 7.6, and Beej 9.
 */

#include <stdio.h>

#include "clings.h"

int write_numbers(const char *path, int n) {
    // TODO: write 1..n to `path`, one per line; return 0 on success.
    (void)path;
    (void)n;
    return -1;
}

int sum_file(const char *path) {
    // TODO: read numbers from `path` until fscanf stops returning 1; return the sum.
    (void)path;
    return 0;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(write_numbers("fileio2_out.txt", 5), 0);
    CHECK_EQ_INT(sum_file("fileio2_out.txt"), 15);
    return clings_summary();
}
