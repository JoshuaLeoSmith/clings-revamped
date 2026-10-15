#ifndef CLINGS_BUF_H
#define CLINGS_BUF_H

#include <stdbool.h>
#include <stddef.h>

struct buf {
    char *data;
    size_t len, cap;
};

void buf_init(struct buf *b);
void buf_free(struct buf *b);
void buf_append(struct buf *b, const void *p, size_t n);
void buf_appends(struct buf *b, const char *s);
void buf_printf(struct buf *b, const char *fmt, ...);
void buf_clear(struct buf *b);

bool file_read(const char *path, struct buf *out);
bool file_write(const char *path, const void *p, size_t n);
bool file_exists(const char *path);
bool mkdir_p(const char *path);

void die(const char *fmt, ...);
char *xstrdup(const char *s);
void *xmalloc(size_t n);

#endif
