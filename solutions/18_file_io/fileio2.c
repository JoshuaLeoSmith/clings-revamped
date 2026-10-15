// Fix: mode "w" creates (or empties) the file for writing; `fscanf` returns
// the number of values it converted, so loop while it returns 1.
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
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        return -1;
    }
    for (int i = 1; i <= n; i++) {
        fprintf(f, "%d\n", i);
    }
    fclose(f);
    return 0;
}

int sum_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    int sum = 0;
    int x;
    while (fscanf(f, "%d", &x) == 1) {
        sum += x;
    }
    fclose(f);
    return sum;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(write_numbers("fileio2_out.txt", 5), 0);
    CHECK_EQ_INT(sum_file("fileio2_out.txt"), 15);
    return clings_summary();
}
