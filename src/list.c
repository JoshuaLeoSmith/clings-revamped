#include "list.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void list_render(const struct app_state *st, struct buf *out) {
    for (size_t i = 0; i < st->info->n; i++) {
        buf_printf(out, "%s%3zu  %s  %s\n", i == st->current ? "> " : "  ", i + 1,
                   st->done[i] ? "DONE   " : "PENDING", st->info->exercises[i].path);
    }
}

/* Read one line from stdin with read(2) (no stdio buffering, so nothing is
 * left behind for a later poll). Drops the newline. False on EOF/error
 * before any byte. */
static bool read_line(struct buf *line) {
    buf_clear(line);
    bool got = false;
    for (;;) {
        char c;
        ssize_t r = read(STDIN_FILENO, &c, 1);
        if (r < 0 && errno == EINTR) continue;
        if (r <= 0) break;
        got = true;
        if (c == '\n') break;
        buf_append(line, &c, 1);
    }
    buf_append(line, "", 1); /* NUL-terminate */
    line->len--;
    return got;
}

/* Index for s: a 1-based number or an exercise name; -1 if neither. */
static long parse_choice(const struct app_state *st, const char *s) {
    if (*s >= '0' && *s <= '9') {
        char *end;
        errno = 0;
        unsigned long n = strtoul(s, &end, 10);
        if (*end || errno || n < 1 || n > st->info->n) return -1;
        return (long)(n - 1);
    }
    return info_find(st->info, s);
}

bool list_prompt_jump(struct app_state *st) {
    fputs("\nJump to (name or number, Enter to cancel): ", stdout);
    fflush(stdout);

    struct buf line;
    buf_init(&line);
    read_line(&line);
    /* trim */
    char *s = line.data;
    while (*s == ' ' || *s == '\t' || *s == '\r') s++;
    size_t n = strlen(s);
    while (n && (s[n - 1] == ' ' || s[n - 1] == '\t' || s[n - 1] == '\r')) s[--n] = '\0';

    bool changed = false;
    if (*s) {
        long idx = parse_choice(st, s);
        if (idx < 0) {
            printf("No exercise \"%s\"\nPress ENTER to continue ", s);
            fflush(stdout);
            read_line(&line);
        } else if ((size_t)idx != st->current) {
            state_set_current(st, (size_t)idx);
            changed = true;
        }
    }
    buf_free(&line);
    return changed;
}
