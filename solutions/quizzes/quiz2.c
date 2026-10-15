// Fix: arrays of structs are walked by index; a string is walked until its
// '\0', and shortened by writing a new '\0' over the last char.
/*
 * quiz2
 *
 * This quiz covers arrays, strings, pointers, structs, enums and typedef
 * (topics 09 to 13). Write these four functions above `main`:
 *
 * - `highest_score(s, n)`: return the highest `score` among the `n`
 *   students in the array `s` (n is at least 1).
 * - `average_score(s, n)`: return the mean of their scores as a `double`.
 * - `count_char(s, c)`: return how many times the char `c` occurs in the
 *   string `s`.
 * - `trim_newline(s)`: if the string `s` ends with '\n', remove that one
 *   '\n' (by moving the end of the string); otherwise leave `s` unchanged.
 *
 * See K&R 5.5 and 6.3, and Beej 7 and 8.
 */

#include <string.h>

#include "clings.h"

struct Student {
    char name[32];
    int score;
};

int highest_score(const struct Student s[], int n) {
    int best = s[0].score;
    for (int i = 1; i < n; i++) {
        if (s[i].score > best) {
            best = s[i].score;
        }
    }
    return best;
}

double average_score(const struct Student s[], int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += s[i].score;
    }
    return (double)total / n;
}

int count_char(const char *s, char c) {
    int count = 0;
    for (; *s != '\0'; s++) {
        if (*s == c) {
            count++;
        }
    }
    return count;
}

void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

int main(void) {
    struct Student students[] = {{"Ada", 90}, {"Grace", 85}, {"Linus", 97}};
    char line[] = "hello\n";
    char word[] = "hi";
    trim_newline(line);
    trim_newline(word);

    // Don't change the tests!
    CHECK_EQ_INT(highest_score(students, 3), 97);
    CHECK_EQ_DBL(average_score(students, 3), 272.0 / 3.0);
    CHECK_EQ_INT(count_char("banana", 'a'), 3);
    CHECK_EQ_INT(count_char("", 'x'), 0);
    CHECK_EQ_STR(line, "hello");
    CHECK_EQ_STR(word, "hi");
    return clings_summary();
}
