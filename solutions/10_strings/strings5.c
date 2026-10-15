// Fix: swap s[i] with s[len - 1 - i] up to the middle. toupper takes an int
// that must fit an unsigned char, so a plain (possibly negative) char is cast first.
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
    size_t len = strlen(s);
    for (size_t i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

void to_upper(char s[]) {
    for (size_t i = 0; s[i] != '\0'; i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }
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
