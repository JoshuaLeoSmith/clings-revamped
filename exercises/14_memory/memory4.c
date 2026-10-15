/*
 * memory4
 *
 * A growable array: `data` holds `cap` ints, of which the first `len` are in
 * use. Write `vec_push`: when the buffer is full, grow it with `realloc`
 * (start at 4 ints and double each time), then store the value at the end.
 * `realloc(NULL, n)` works like `malloc(n)`, so the empty vector needs no
 * special case. `vec_free` gives the buffer back when you are done.
 * See K&R 7.8.5 and Beej 12.
 */

#include <stdlib.h>

#include "clings.h"

struct IntVec {
    int *data;
    size_t len, cap;
};

void vec_push(struct IntVec *v, int value) {
    // TODO: grow `v->data` when it is full, then append `value`.
    (void)v;
    (void)value;
}

void vec_free(struct IntVec *v) {
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
}

int main(void) {
    struct IntVec v = {NULL, 0, 0};
    for (int i = 0; i < 100; i++) {
        vec_push(&v, i);
    }

    // Don't change the tests!
    CHECK_EQ_INT(v.len, 100);
    if (v.len == 100) {
        CHECK_EQ_INT(v.data[0], 0);
        CHECK_EQ_INT(v.data[99], 99);
    }
    CHECK(v.cap >= v.len);
    vec_free(&v);
    return clings_summary();
}
