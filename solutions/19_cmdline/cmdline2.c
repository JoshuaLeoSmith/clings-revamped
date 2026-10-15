// Fix: `strtol` never fails loudly; the caller checks `endptr` (nothing read, or
// junk left over) and `errno == ERANGE` (too large for a long) itself.
/*
 * cmdline2
 *
 * Command-line arguments are strings, so numbers must be parsed. `strtol`
 * converts a string to a `long` and sets `endptr` to the first character it
 * did not use: if that is the start of the string nothing was read ("abc"),
 * and if it is not '\0' there is junk after the number ("12x"). Write
 * `parse_and_sum`: add up argv[1..], store the sum in `*out` and return
 * EXIT_SUCCESS, or return EXIT_FAILURE on the first bad argument.
 * See K&R 5.10 and B5, and Beej 18.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "clings.h"

int parse_and_sum(int argc, char **argv, long *out) {
    long sum = 0;
    for (int i = 1; i < argc; i++) {
        char *end;
        errno = 0;
        long value = strtol(argv[i], &end, 10);
        if (end == argv[i] || *end != '\0' || errno == ERANGE) {
            return EXIT_FAILURE;
        }
        sum += value;
    }
    *out = sum;
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    // Run this from your IDE with numbers as "Program arguments" to try it.
    long total = 0;
    if (parse_and_sum(argc, argv, &total) == EXIT_SUCCESS) {
        printf("Your arguments add up to %ld\n", total);
    } else {
        printf("Your arguments are not all numbers\n");
    }

    // Don't change the tests!
    char *good[] = {"p", "10", "32", NULL};
    char *word[] = {"p", "abc", NULL};
    char *junk[] = {"p", "12x", NULL};
    long out = 0;
    CHECK_EQ_INT(parse_and_sum(3, good, &out), EXIT_SUCCESS);
    CHECK_EQ_INT(out, 42);
    CHECK_EQ_INT(parse_and_sum(2, word, &out), EXIT_FAILURE);
    CHECK_EQ_INT(parse_and_sum(2, junk, &out), EXIT_FAILURE);
    return clings_summary();
}
