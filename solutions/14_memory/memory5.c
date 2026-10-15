// Fix: `strlen` counts the chars before the '\0', so a copy needs
// `strlen(s) + 1` bytes; forgetting the + 1 is the classic heap overflow.
/*
 * memory5
 *
 * `my_strdup` makes a copy of a string on the heap; the caller frees it.
 * But a string needs one more byte than `strlen` counts, for the '\0' at
 * the end, and `strcpy` writes that byte past the end of the block. The
 * address sanitizer reports the overflow. Allocate enough room.
 * See K&R 7.8.5 and Beej 12.
 */

#include <stdlib.h>
#include <string.h>

#include "clings.h"

char *my_strdup(const char *s) {
    char *copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, s);
    return copy;
}

int main(void) {
    const char *original = "memory";
    char *copy = my_strdup(original);

    // Don't change the tests!
    CHECK_EQ_STR(copy, "memory");
    CHECK(copy != original);
    free(copy);
    return clings_summary();
}
