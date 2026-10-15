#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "clings.h"

int main(void) {
    CHECK_EQ_INT(1, 1);
    CHECK_EQ_STR("a", "a");
    CHECK_EQ_DBL(0.1 + 0.2, 0.3);
    CHECK_EQ_CHAR('x', 'x');

    /* Failing checks write their FAIL lines through to the file at once. */
    char tmp[] = "/tmp/clings_h_XXXXXX";
    int fd = mkstemp(tmp);
    int saved = dup(STDOUT_FILENO);
    fflush(stdout);
    if (fd < 0 || saved < 0 || dup2(fd, STDOUT_FILENO) < 0) {
        printf("cannot redirect stdout\n");
        return 1;
    }
    CHECK_EQ_INT(1, 2);
    CHECK_EQ_STR(NULL, "a");
    char text[512] = {0};
    ssize_t got = pread(fd, text, sizeof text - 1, 0); /* no fflush: the macros flush */
    dup2(saved, STDOUT_FILENO);
    close(saved);
    close(fd);
    unlink(tmp);
    if (got <= 0 || !strstr(text, "FAIL ") || !strstr(text, "1 is 1, expected 2") ||
        !strstr(text, "NULL is NULL, expected \"a\"")) {
        printf("FAIL lines not written: '%s'\n", got > 0 ? text : "");
        return 1;
    }
    if (clings_failed != 2) {
        printf("expected 2 failures, got %d\n", clings_failed);
        return 1;
    }
    clings_failed = 0;
    int n = 0;
    CHECK_EQ_INT(n++, 0);
    CHECK_EQ_INT(n, 1);
    return clings_summary();
}
