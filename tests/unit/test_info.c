#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clings.h"
#include "buf.h"
#include "info.h"

#define HDR "format_version = 1\n"
#define EX1 "[[exercises]]\nname = \"intro1\"\ndir = \"00_intro\"\nhint = \"h\"\n"

static void expect_err(const char *text, const char *prefix, const char *frag) {
    struct info_file f = {0};
    char err[256] = "";
    bool ok = info_parse(text, &f, err, sizeof err);
    CHECK(!ok);
    if (strncmp(err, prefix, strlen(prefix)) != 0) {
        clings_failed++;
        printf("FAIL %s:%d: err \"%s\" should start with \"%s\"\n", __FILE__,
               __LINE__, err, prefix);
    } else {
        clings_passed++;
    }
    if (!strstr(err, frag)) {
        clings_failed++;
        printf("FAIL %s:%d: err \"%s\" should contain \"%s\"\n", __FILE__,
               __LINE__, err, frag);
    } else {
        clings_passed++;
    }
    info_free(&f);
}

static void test_minimal(void) {
    struct info_file f = {0};
    char err[256] = "";
    CHECK(info_parse(HDR EX1, &f, err, sizeof err));
    CHECK_EQ_INT(f.format_version, 1);
    CHECK_EQ_INT((long long)f.n, 1);
    CHECK_EQ_STR(f.welcome_message, "");
    CHECK_EQ_STR(f.final_message, "");
    CHECK_EQ_STR(f.exercises[0].name, "intro1");
    CHECK_EQ_STR(f.exercises[0].dir, "00_intro");
    CHECK_EQ_STR(f.exercises[0].path, "exercises/00_intro/intro1.c");
    CHECK_EQ_STR(f.exercises[0].hint, "h");
    CHECK(!f.exercises[0].strict);
    CHECK(!f.exercises[0].skip_check_unsolved);
    CHECK(f.exercises[0].stdin_text == NULL);
    CHECK_EQ_INT((long long)f.exercises[0].n_sources, 0);
    info_free(&f);
    CHECK(f.exercises == NULL && f.n == 0);
}

static void test_all_fields(void) {
    const char *t =
        "# top comment\n"
        "\n"
        "format_version = 1 # trailing\n"
        "welcome_message = \"Hi # not a comment\"\n"
        "final_message = \"Bye\\n\"\n"
        "\n"
        "[[exercises]]\n"
        "name = \"io1\"  # c\n"
        "dir = \"05_io\"\n"
        "hint = \"say \\\"hi\\\"\"\n"
        "strict = true\n"
        "stdin = \"x\\ny\\n\"\n"
        "sources = [\"a.c\", \"b.c\"]\n"
        "skip_check_unsolved = true\n"
        "\n"
        "[[exercises]]\n"
        "name = \"io2\"\ndir = \"05_io\"\nhint = \"t\\tab\"\n";
    struct info_file f = {0};
    char err[256] = "";
    CHECK(info_parse(t, &f, err, sizeof err));
    CHECK_EQ_INT((long long)f.n, 2);
    CHECK_EQ_STR(f.welcome_message, "Hi # not a comment");
    CHECK_EQ_STR(f.final_message, "Bye\n");
    struct exercise_info *e = &f.exercises[0];
    CHECK_EQ_STR(e->name, "io1");
    CHECK_EQ_STR(e->hint, "say \"hi\"");
    CHECK(e->strict);
    CHECK(e->skip_check_unsolved);
    CHECK_EQ_STR(e->stdin_text, "x\ny\n");
    CHECK_EQ_INT((long long)e->n_sources, 2);
    if (e->n_sources == 2) {
        CHECK_EQ_STR(e->sources[0], "a.c");
        CHECK_EQ_STR(e->sources[1], "b.c");
    }
    CHECK_EQ_STR(f.exercises[1].hint, "t\tab");
    CHECK(!f.exercises[1].strict);
    CHECK_EQ_INT(info_find(&f, "io1"), 0);
    CHECK_EQ_INT(info_find(&f, "io2"), 1);
    CHECK_EQ_INT(info_find(&f, "nope"), -1);
    info_free(&f);
}

static void test_multiline(void) {
    const char *t = HDR
        "[[exercises]]\nname = \"m\"\ndir = \"d\"\n"
        "hint = \"\"\"\n"
        "line one # kept\n"
        "say \"quoted\" and back\\\\slash\n"
        "\"\"\"\n";
    struct info_file f = {0};
    char err[256] = "";
    CHECK(info_parse(t, &f, err, sizeof err));
    CHECK_EQ_INT((long long)f.n, 1);
    if (f.n == 1)
        CHECK_EQ_STR(f.exercises[0].hint,
                     "line one # kept\nsay \"quoted\" and back\\slash\n");
    info_free(&f);

    /* inline """ on one line, then another key after it */
    const char *t2 = HDR
        "[[exercises]]\nname = \"m\"\ndir = \"d\"\nhint = \"\"\"abc\"\"\"\nstrict = true\n";
    CHECK(info_parse(t2, &f, err, sizeof err));
    if (f.n == 1) {
        CHECK_EQ_STR(f.exercises[0].hint, "abc");
        CHECK(f.exercises[0].strict);
    }
    info_free(&f);
}

static void test_closest(void) {
    struct info_file f = {0};
    char err[256] = "";
    CHECK(info_parse(HDR EX1
                     "[[exercises]]\nname = \"vars1\"\ndir = \"d\"\nhint = \"h\"\n"
                     "[[exercises]]\nname = \"the_intro\"\ndir = \"d\"\nhint = \"h\"\n",
                     &f, err, sizeof err));
    struct buf b;
    buf_init(&b);
    info_closest(&f, "intr", &b);
    CHECK_EQ_STR(b.data, "intro1\nthe_intro\n");
    buf_clear(&b);
    info_closest(&f, "vax", &b);
    CHECK(b.data && strstr(b.data, "vars1") != NULL);
    buf_free(&b);
    info_free(&f);
}

static void test_errors(void) {
    expect_err(HDR "[[exercises]]\ndir = \"d\"\nhint = \"h\"\n", "2:", "name");
    expect_err(HDR "[[exercises]]\nname = \"a\"\nhint = \"h\"\n", "2:", "dir");
    expect_err(HDR "[[exercises]]\nname = \"a\"\ndir = \"d\"\n", "2:", "hint");
    expect_err(HDR EX1 "[[exercises]]\nname = \"intro1\"\ndir = \"d\"\nhint = \"h\"\n",
               "7:", "duplicate exercise name \"intro1\"");
    expect_err(HDR "[[exercises]]\nname = \"a-b\"\ndir = \"d\"\nhint = \"h\"\n",
               "3:", "invalid exercise name");
    expect_err(HDR "[[exercises]]\nname = \"aaaaaaaaaabbbbbbbbbbccccccccccddd\"\n"
               "dir = \"d\"\nhint = \"h\"\n", "3:", "too long");
    expect_err(HDR EX1 "foo = 1\n", "6:", "unknown key \"foo\"");
    expect_err("foo = 1\n" HDR EX1, "1:", "unknown key");
    expect_err(HDR "welcome_message = \"abc\n" EX1, "2:", "unterminated");
    expect_err(HDR "[[exercises]]\nname = \"a\"\nhint = \"\"\"abc\n", "4:", "unterminated");
    expect_err(HDR "just some words\n" EX1, "2:", "=");
    expect_err(HDR "welcome_message = \"a\\qb\"\n" EX1, "2:", "escape");
    expect_err("format_version = 2\n" EX1, "1:", "unsupported format_version 2");
    expect_err(EX1, "", "format_version");
    expect_err(HDR, "", "no exercises");
    expect_err(HDR EX1 "[[other]]\n", "6:", "[[");
    expect_err(HDR "[[exercises]]\nname = \"a\"\nsources = [\"a\", 3]\n", "4:", "array");
    expect_err(HDR "[[exercises]]\nname = \"a\"\nstrict = maybe\n", "4:", "boolean");
    expect_err(HDR "[[exercises]]\nname = 5\n", "3:", "string");
}

static void test_load(void) {
    struct info_file f = {0};
    char err[256] = "";
    CHECK(!info_load("/nonexistent/info.toml", &f, err, sizeof err));
    CHECK(err[0] != '\0');

    char path[] = "/tmp/clings_info_XXXXXX";
    int fd = mkstemp(path);
    CHECK(fd >= 0);
    if (fd < 0) return;
    const char *t = HDR "bogus line\n";
    CHECK(file_write(path, t, strlen(t)));
    CHECK(!info_load(path, &f, err, sizeof err));
    CHECK(strstr(err, "info.toml:2:") != NULL);
    t = HDR EX1;
    CHECK(file_write(path, t, strlen(t)));
    CHECK(info_load(path, &f, err, sizeof err));
    CHECK_EQ_INT((long long)f.n, 1);
    info_free(&f);
    remove(path);
}

int main(void) {
    test_minimal();
    test_all_fields();
    test_multiline();
    test_closest();
    test_errors();
    test_load();
    return clings_summary();
}
