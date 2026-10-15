#include <stdlib.h>
#include <string.h>
#include "buf.h"
#include "state.h"

#define STATE_HEADER "DON'T EDIT THIS FILE!"

/* Index of the exercise named name[0..len), or -1. */
static long find_n(const struct info_file *info, const char *name, size_t len) {
    for (size_t i = 0; i < info->n; i++) {
        const char *e = info->exercises[i].name;
        if (strlen(e) == len && memcmp(e, name, len) == 0) return (long)i;
    }
    return -1;
}

void state_load(struct app_state *st, const struct info_file *info,
                const char *path, bool *fresh) {
    st->info = info;
    st->path = path;
    st->current = 0;
    st->n_done = 0;
    st->done = xmalloc((info->n ? info->n : 1) * sizeof *st->done);
    for (size_t i = 0; i < info->n; i++) st->done[i] = false;
    *fresh = true;

    struct buf b;
    buf_init(&b);
    if (!file_read(path, &b) || b.len == 0) {
        buf_free(&b);
        return;
    }

    /* Split into lines (a final unterminated line still counts). */
    const char *p = b.data, *end = b.data + b.len;
    size_t lineno = 0;
    bool ok = false;
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        size_t len = nl ? (size_t)(nl - p) : (size_t)(end - p);
        if (lineno == 2) {
            long c = find_n(info, p, len);
            if (c < 0) break;
            st->current = (size_t)c;
            ok = true;
        } else if (lineno >= 4 && len > 0) {
            long d = find_n(info, p, len);
            if (d >= 0 && !st->done[d]) {
                st->done[d] = true;
                st->n_done++;
            }
        }
        lineno++;
        p = nl ? nl + 1 : end;
    }
    buf_free(&b);

    if (!ok) {
        st->current = 0;
        st->n_done = 0;
        for (size_t i = 0; i < info->n; i++) st->done[i] = false;
        return;
    }
    *fresh = false;
}

bool state_save(const struct app_state *st) {
    struct buf b;
    buf_init(&b);
    buf_appends(&b, STATE_HEADER "\n\n");
    if (st->info->n > 0) buf_appends(&b, st->info->exercises[st->current].name);
    buf_appends(&b, "\n\n");
    for (size_t i = 0; i < st->info->n; i++) {
        if (!st->done[i]) continue;
        buf_appends(&b, st->info->exercises[i].name);
        buf_appends(&b, "\n");
    }
    bool ok = file_write(st->path, b.data, b.len);
    buf_free(&b);
    return ok;
}

bool state_set_current(struct app_state *st, size_t idx) {
    if (idx >= st->info->n) return false;
    st->current = idx;
    state_save(st);
    return true;
}

void state_mark_done(struct app_state *st, size_t idx) {
    if (idx >= st->info->n || st->done[idx]) return;
    st->done[idx] = true;
    st->n_done++;
    state_save(st);
}

void state_mark_pending(struct app_state *st, size_t idx) {
    if (idx >= st->info->n || !st->done[idx]) return;
    st->done[idx] = false;
    st->n_done--;
    state_save(st);
}

bool state_next_pending(const struct app_state *st, size_t *idx) {
    size_t n = st->info->n;
    for (size_t k = 1; k < n; k++) {
        size_t i = (st->current + k) % n;
        if (!st->done[i]) {
            *idx = i;
            return true;
        }
    }
    return false;
}

void state_free(struct app_state *st) {
    free(st->done);
    st->done = NULL;
}
