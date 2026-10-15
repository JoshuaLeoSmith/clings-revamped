/*
 * text1
 *
 * <ctype.h> classifies and converts single characters: `isdigit`,
 * `isalpha`, `isspace`, `tolower`, `toupper` and friends. Use them to write
 * `count_vowels` (a, e, i, o, u in either case), `count_digits`, and
 * `capitalize_words`, which makes the first letter of each word upper case
 * and the rest lower case (words are separated by spaces).
 * See K&R 7.8.2 and Beej 21.
 */

#include <ctype.h>

#include "clings.h"

int count_vowels(const char *s) {
    // TODO: count the vowels in `s`.
    (void)s;
    return 0;
}

int count_digits(const char *s) {
    // TODO: count the digits in `s`.
    (void)s;
    return 0;
}

void capitalize_words(char *s) {
    // TODO: "hello wORLD" becomes "Hello World".
    (void)s;
}

int main(void) {
    char title[] = "hello wORLD";
    capitalize_words(title);

    // Don't change the tests!
    CHECK_EQ_INT(count_vowels("Hello World"), 3);
    CHECK_EQ_INT(count_digits("a1b22"), 3);
    CHECK_EQ_STR(title, "Hello World");
    return clings_summary();
}
