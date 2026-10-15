#include "buf.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void die(const char *fmt, ...) {
    va_list ap;
    fputs("clings: ", stderr);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(2);
}

void *xmalloc(size_t n) {
    void *p = malloc(n ? n : 1);
    if (!p)
        die("out of memory");
    return p;
}

char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    return memcpy(xmalloc(n), s, n);
}

void buf_init(struct buf *b) {
    b->cap = 64;
    b->len = 0;
    b->data = xmalloc(b->cap);
    b->data[0] = '\0';
}

void buf_free(struct buf *b) {
    free(b->data);
    b->data = NULL;
    b->len = b->cap = 0;
}

static void buf_reserve(struct buf *b, size_t extra) {
    size_t need = b->len + extra + 1;
    if (need <= b->cap)
        return;
    size_t cap = b->cap ? b->cap : 64;
    while (cap < need)
        cap *= 2;
    char *p = realloc(b->data, cap);
    if (!p)
        die("out of memory");
    b->data = p;
    b->cap = cap;
}

void buf_append(struct buf *b, const void *p, size_t n) {
    buf_reserve(b, n);
    if (n)
        memcpy(b->data + b->len, p, n);
    b->len += n;
    b->data[b->len] = '\0';
}

void buf_appends(struct buf *b, const char *s) {
    buf_append(b, s, strlen(s));
}

void buf_printf(struct buf *b, const char *fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0)
        die("vsnprintf failed");
    buf_reserve(b, (size_t)n);
    vsnprintf(b->data + b->len, (size_t)n + 1, fmt, ap2);
    va_end(ap2);
    b->len += (size_t)n;
}

void buf_clear(struct buf *b) {
    b->len = 0;
    if (b->data)
        b->data[0] = '\0';
}

bool file_read(const char *path, struct buf *out) {
    FILE *f = fopen(path, "rb");
    if (!f)
        return false;
    char chunk[4096];
    size_t n;
    while ((n = fread(chunk, 1, sizeof chunk, f)) > 0)
        buf_append(out, chunk, n);
    bool ok = !ferror(f);
    fclose(f);
    return ok;
}

bool file_write(const char *path, const void *p, size_t n) {
    FILE *f = fopen(path, "wb");
    if (!f)
        return false;
    bool ok = n == 0 || fwrite(p, 1, n, f) == n;
    if (fclose(f) != 0)
        ok = false;
    return ok;
}

bool file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

bool mkdir_p(const char *path) {
    char *tmp = xstrdup(path);
    bool ok = true;
    for (char *p = tmp + 1; ok; p++) {
        if (*p == '/' || *p == '\0') {
            char saved = *p;
            *p = '\0';
            if (mkdir(tmp, 0777) != 0 && errno != EEXIST)
                ok = false;
            *p = saved;
            if (saved == '\0')
                break;
        }
    }
    if (ok) {
        struct stat st;
        ok = stat(tmp, &st) == 0 && S_ISDIR(st.st_mode);
    }
    free(tmp);
    return ok;
}
