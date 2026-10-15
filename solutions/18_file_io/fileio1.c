// Fix: always check `fopen` for NULL and `fclose` what you opened. Each `fgets`
// call reads up to a newline, so a last line without one still counts (a line
// longer than the buffer would be split and counted twice, which is fine here).
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
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    char line[256];
    int count = 0;
    while (fgets(line, sizeof line, f) != NULL) {
        count++;
    }
    fclose(f);
    return count;
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_INT(count_lines("fileio1.txt"), 5);
    CHECK_EQ_INT(count_lines("missing.txt"), -1);
    return clings_summary();
}
