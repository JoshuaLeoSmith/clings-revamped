#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "clings.h"
#include "buf.h"

static void test_append(void) {
    struct buf b;
    buf_init(&b);
    buf_append(&b, "abc", 3);
    CHECK_EQ_INT((long long)b.len, 3);
    CHECK(memcmp(b.data, "abc", 3) == 0);
    CHECK_EQ_CHAR(b.data[3], '\0');
    buf_appends(&b, "def");
    CHECK_EQ_STR(b.data, "abcdef");
    buf_free(&b);
}

static void test_printf(void) {
    struct buf b;
    buf_init(&b);
    buf_printf(&b, "n=%d s=%s", 42, "hi");
    CHECK_EQ_STR(b.data, "n=42 s=hi");
    buf_printf(&b, "!");
    CHECK_EQ_STR(b.data, "n=42 s=hi!");
    char big[3000];
    memset(big, 'x', sizeof big - 1);
    big[sizeof big - 1] = '\0';
    buf_clear(&b);
    buf_printf(&b, "%s", big);
    CHECK_EQ_INT((long long)b.len, 2999);
    CHECK_EQ_STR(b.data, big);
    buf_free(&b);
}

static void test_growth_clear(void) {
    struct buf b;
    buf_init(&b);
    for (int i = 0; i < 100 * 1024; i++) {
        char c = (char)('a' + i % 26);
        buf_append(&b, &c, 1);
    }
    CHECK_EQ_INT((long long)b.len, 100 * 1024);
    CHECK_EQ_CHAR(b.data[100 * 1024], '\0');
    CHECK_EQ_CHAR(b.data[27], 'b');
    buf_clear(&b);
    CHECK_EQ_INT((long long)b.len, 0);
    CHECK_EQ_CHAR(b.data[0], '\0');
    buf_free(&b);
}

static void test_files(void) {
    char dir[] = "/tmp/clings_test_XXXXXX";
    if (!mkdtemp(dir)) {
        perror("mkdtemp");
        exit(1);
    }
    char path[256];
    snprintf(path, sizeof path, "%s/f.bin", dir);
    const char data[] = {'a', 'b', '\0', 'c', '\xff'};
    CHECK(!file_exists(path));
    CHECK(file_write(path, data, sizeof data));
    CHECK(file_exists(path));
    struct buf b;
    buf_init(&b);
    CHECK(file_read(path, &b));
    CHECK_EQ_INT((long long)b.len, (long long)sizeof data);
    CHECK(memcmp(b.data, data, sizeof data) == 0);
    CHECK_EQ_CHAR(b.data[b.len], '\0');
    buf_free(&b);

    buf_init(&b);
    char missing[300];
    snprintf(missing, sizeof missing, "%s/missing", dir);
    CHECK(!file_read(missing, &b));
    buf_free(&b);

    char nested[300];
    snprintf(nested, sizeof nested, "%s/tmp/a/b/c", dir);
    CHECK(mkdir_p(nested));
    struct stat st;
    CHECK(stat(nested, &st) == 0 && S_ISDIR(st.st_mode));
    CHECK(mkdir_p(nested));

    char p[400];
    const char *subs[] = {"tmp/a/b/c", "tmp/a/b", "tmp/a", "tmp"};
    for (int i = 0; i < 4; i++) {
        snprintf(p, sizeof p, "%s/%s", dir, subs[i]);
        rmdir(p);
    }
    unlink(path);
    rmdir(dir);
}

static void test_x(void) {
    char *s = xstrdup("hello");
    CHECK_EQ_STR(s, "hello");
    free(s);
    void *m = xmalloc(16);
    CHECK(m != NULL);
    free(m);
}

int main(void) {
    test_append();
    test_printf();
    test_growth_clear();
    test_files();
    test_x();
    return clings_summary();
}
