/*
 * fileio1
 *
 * `fopen` opens a file and gives you a `FILE *`, or NULL if it could not;
 * `fgets` reads one line at a time and returns NULL at the end of the
 * file; `fclose` closes it again. Write `count_lines` so it returns the
 * number of lines in the file, or -1 if the file cannot be opened.
 * See K&R 7.5 and 7.7, and Beej 9.
 */

#include <stdio.h>

#include "clings.h"

int count_lines(const char *path) {
    // TODO: open `path`, count the lines with fgets, close it, return the count.
    (void)path;
    return -1;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(count_lines("fileio1.txt"), 5);
    CHECK_EQ_INT(count_lines("missing.txt"), -1);
    return clings_summary();
}
