/*
 * strings5
 *
 * A string in a `char` array can be changed in place. Implement `reverse`,
 * which reverses the chars before the '\0' (swap the first and last, then
 * move inwards), and `to_upper`, which turns every lowercase letter into
 * its uppercase form. The '\0' stays where it is.
 * See K&R 1.9 and Beej 21.
 */

#include <ctype.h>
#include <string.h>

#include "clings.h"

void reverse(char s[]) {
    // TODO: reverse s in place.
    (void)s;
}

void to_upper(char s[]) {
    // TODO: make every letter in s uppercase.
    (void)s;
}

int main(void) {
    char word[] = "hello";
    char two[] = "ab";
    char empty[] = "";

    // Don't change the tests!
    reverse(word);
    CHECK_EQ_STR(word, "olleh");
    to_upper(word);
    CHECK_EQ_STR(word, "OLLEH");
    reverse(two);
    CHECK_EQ_STR(two, "ba");
    reverse(empty);
    CHECK_EQ_STR(empty, "");
    return clings_summary();
}
