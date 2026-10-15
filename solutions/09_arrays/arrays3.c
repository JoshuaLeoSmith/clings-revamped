// Fix: an array of 5 has indexes 0 to 4, so the loop runs while i < 5. C never
// checks indexes; writing values[5] would overwrite whatever follows the array.
/*
 * arrays3
 *
 * `int values[5]` has the indexes 0 to 4. C does not check indexes, so this
 * loop writes one element past the end and overwrites whatever is next in
 * memory. Here the sanitizer stops the program with "index 5 out of bounds
 * for type 'int [5]'"; read its report, then fix the loop.
 * See K&R 1.6 and Beej 6.
 */

#include "clings.h"

int main(void) {
    int values[5];
    for (int i = 0; i < 5; i++) {
        values[i] = i;
    }

    // Don't change the tests!
    CHECK_EQ_INT(values[4], 4);
    return clings_summary();
}
