#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clings.h"
#include "buf.h"
#include "term.h"

/* Renders into a fresh buf and returns a NUL-terminated copy. */
static char *bar(size_t done, size_t total, int width) {
    struct buf b;
    buf_init(&b);
    term_progress_bar(&b, done, total, width);
    buf_append(&b, "", 1);
    char *s = xstrdup(b.data);
    buf_free(&b);
    return s;
}

static char *link_of(const char *rel) {
    struct buf b;
    buf_init(&b);
    term_file_link(&b, rel);
    buf_append(&b, "", 1);
    char *s = xstrdup(b.data);
    buf_free(&b);
    return s;
}

static bool starts_with(const char *s, const char *p) {
    return strncmp(s, p, strlen(p)) == 0;
}

static bool ends_with(const char *s, const char *p) {
    size_t n = strlen(s), m = strlen(p);
    return n >= m && strcmp(s + n - m, p) == 0;
}

int main(void) {
    char *s;

    /* --- colours off --- */
    term_force_colors(false);
    CHECK(!term_colors_enabled());

    s = bar(0, 10, 30);
    CHECK_EQ_STR(s, "Progress: [>---------]   0/10");
    free(s);

    s = bar(10, 10, 30);
    CHECK_EQ_STR(s, "Progress: [##########]  10/10");
    free(s);

    char want[128] = "Progress: [##>";
    for (int i = 0; i < 57; i++) strcat(want, "-");
    strcat(want, "]   3/88");
    s = bar(3, 88, 80);
    CHECK_EQ_STR(s, want);
    free(s);

    /* narrow width fallback */
    s = bar(5, 10, 20);
    CHECK_EQ_STR(s, "Progress: 5/10");
    free(s);

    /* total == 0 does not divide by zero */
    s = bar(0, 0, 30);
    CHECK_EQ_STR(s, "Progress: [>---------]   0/0");
    free(s);

    s = link_of("exercises/x.c");
    CHECK_EQ_STR(s, "exercises/x.c");
    free(s);

    struct buf b;
    buf_init(&b);
    term_color(&b, TERM_RED);
    CHECK_EQ_INT(b.len, 0);
    buf_free(&b);

    /* --- colours on --- */
    term_force_colors(true);
    CHECK(term_colors_enabled());

    s = link_of("include/clings.h");
    const char *pre = "\x1b]8;;file://";
    const char *mid = "\x1b\\include/clings.h";
    const char *suf = "\x1b]8;;\x1b\\";
    CHECK(starts_with(s, pre));
    CHECK(ends_with(s, suf));
    /* the absolute path is followed by ST then the visible text */
    char *m = strstr(s, mid);
    CHECK(m != NULL);
    if (m) {
        CHECK(strcmp(m + strlen(mid), suf) == 0);
        size_t plen = (size_t)(m - (s + strlen(pre)));
        char abs[4096];
        CHECK(plen < sizeof abs);
        if (plen < sizeof abs) {
            memcpy(abs, s + strlen(pre), plen);
            abs[plen] = '\0';
            CHECK(abs[0] == '/');
            CHECK(ends_with(abs, "/include/clings.h"));
        }
    }
    free(s);

    buf_init(&b);
    term_color(&b, TERM_RED);
    buf_append(&b, "", 1);
    CHECK_EQ_STR(b.data, "\x1b[31m");
    buf_clear(&b);
    term_color(&b, TERM_RESET);
    buf_append(&b, "", 1);
    CHECK_EQ_STR(b.data, "\x1b[0m");
    buf_clear(&b);
    term_clear(&b);
    buf_append(&b, "", 1);
    CHECK_EQ_STR(b.data, "\x1b[H\x1b[2J\x1b[3J");
    buf_free(&b);

    /* coloured bar: green run, red run, reset */
    s = bar(5, 10, 30);
    CHECK_EQ_STR(s, "Progress: [\x1b[32m#####>\x1b[31m----\x1b[0m]   5/10");
    free(s);

    term_force_colors(false);

    /* environment-dependent: only check they are safe */
    CHECK(term_width() > 0);
    (void)term_is_tty();
    term_raw_off(); /* off without on is a no-op */
    term_raw_on();
    term_raw_on();
    term_raw_off();
    term_raw_off();

    return clings_summary();
}
