/*
 * quiz3
 *
 * This quiz covers memory, scope, the preprocessor, multifile programs,
 * file I/O, command-line arguments, function pointers, recursion, bits,
 * linked lists and text (topics 14 to 24). Write these functions above
 * `main`:
 *
 * - `stack_init(s)`: make `s` an empty stack (no buffer yet).
 * - `stack_push(s, v)`: put `v` on top; when the buffer is full, grow it
 *   with `realloc` (4 ints first, then double). Return 0, or -1 if the
 *   allocation fails (the stack is then unchanged).
 * - `stack_pop(s, out)`: if the stack is empty return -1; otherwise remove
 *   the top value, store it in `*out` and return 0.
 * - `stack_free(s)`: free the buffer and leave `s` empty.
 * - `join(words, n, sep)`: return a new `malloc`'d string holding the `n`
 *   words with `sep` between them ("" when `n` is 0, never NULL unless
 *   `malloc` fails). The caller frees it.
 *
 * See K&R 5.5, 6.5 and 7.8.5, and Beej 12.
 */

#include <stdlib.h>
#include <string.h>

#include "clings.h"

struct Stack {
    int *data;
    size_t len, cap;
};

// TODO: write the five functions here.
// void stack_init(struct Stack *s) { ??? }
// int stack_push(struct Stack *s, int v) { ??? }
// int stack_pop(struct Stack *s, int *out) { ??? }
// void stack_free(struct Stack *s) { ??? }
// char *join(const char *const *words, int n, const char *sep) { ??? }

int main(void) {
    struct Stack s;
    stack_init(&s);
    int pushed = 0;
    for (int v = 1; v <= 3; v++) {
        if (stack_push(&s, v) == 0) {
            pushed++;
        }
    }
    int a = 0, b = 0, c = 0, d = 0;
    int ra = stack_pop(&s, &a);
    int rb = stack_pop(&s, &b);
    int rc = stack_pop(&s, &c);
    int rd = stack_pop(&s, &d);
    stack_free(&s);

    const char *abc[] = {"a", "b", "c"};
    const char *x[] = {"x"};
    char *joined = join(abc, 3, ", ");
    char *single = join(x, 1, "-");
    char *empty = join(NULL, 0, ",");

    // Don't change the tests!
    CHECK_EQ_INT(pushed, 3);
    CHECK_EQ_INT(ra, 0);
    CHECK_EQ_INT(a, 3);
    CHECK_EQ_INT(rb, 0);
    CHECK_EQ_INT(b, 2);
    CHECK_EQ_INT(rc, 0);
    CHECK_EQ_INT(c, 1);
    CHECK_EQ_INT(rd, -1);
    CHECK_EQ_STR(joined, "a, b, c");
    CHECK_EQ_STR(single, "x");
    CHECK_EQ_STR(empty, "");
    free(joined);
    free(single);
    free(empty);
    return clings_summary();
}
