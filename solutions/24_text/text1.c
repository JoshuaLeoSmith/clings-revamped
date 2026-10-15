// Fix: the <ctype.h> functions take an int that must be EOF or a value of
// `unsigned char`; a plain `char` may be signed, so cast with `(unsigned char)`
// first or a byte above 127 becomes a negative number and undefined behaviour.
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
    int count = 0;
    for (; *s != '\0'; s++) {
        int c = tolower((unsigned char)*s);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_digits(const char *s) {
    int count = 0;
    for (; *s != '\0'; s++) {
        if (isdigit((unsigned char)*s)) {
            count++;
        }
    }
    return count;
}

void capitalize_words(char *s) {
    int start_of_word = 1;
    for (; *s != '\0'; s++) {
        if (*s == ' ') {
            start_of_word = 1;
        } else if (start_of_word) {
            *s = (char)toupper((unsigned char)*s);
            start_of_word = 0;
        } else {
            *s = (char)tolower((unsigned char)*s);
        }
    }
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
