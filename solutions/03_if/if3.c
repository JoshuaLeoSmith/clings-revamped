// Fix: without braces an `if` controls only the single next statement, so
// `return 'A';` ran for every score. Braces group both statements into the body.
/*
 * if3
 *
 * `grade` should return 'A' for 90 and above (and print a cheer), 'B' for
 * 80 and above, and 'C' otherwise. The indentation shows what was meant,
 * but C does not read indentation. Warnings count as errors here.
 * See K&R 3.2-3.3 and Beej 3.3.1.
 */

#include <stdio.h>
#include "clings.h"

char grade(int score) {
    if (score >= 90) {
        printf("Excellent!\n");
        return 'A';
    }
    if (score >= 80) return 'B';
    return 'C';
}

int main(void) {
    // Don't change the tests!
    CHECK_EQ_CHAR(grade(95), 'A');
    CHECK_EQ_CHAR(grade(85), 'B');
    CHECK_EQ_CHAR(grade(50), 'C');
    return clings_summary();
}
