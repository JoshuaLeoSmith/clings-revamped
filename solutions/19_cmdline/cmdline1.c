// Fix: argv holds argc strings plus a NULL, with the program name first;
// `snprintf` never writes more than its size argument and always adds '\0'.
/*
 * cmdline1
 *
 * `main(int argc, char **argv)` receives the command line: `argv[0]` is the
 * program's name and `argv[1]` to `argv[argc - 1]` are the arguments. Write
 * `count_args` (not counting the name) and `join_args`, which joins the
 * arguments with single spaces into `out`, a buffer of `cap` bytes; never
 * write past it, and always end with '\0'. The tests use a fake `argv`.
 * See K&R 5.10 and Beej 18.
 */

#include <stdio.h>

#include "clings.h"

int count_args(int argc, char **argv) {
    int n = 0;
    for (int i = 1; i < argc && argv[i] != NULL; i++) {
        n++;
    }
    return n;
}

void join_args(int argc, char **argv, char *out, size_t cap) {
    if (cap == 0) {
        return;
    }
    out[0] = '\0';
    size_t used = 0;
    for (int i = 1; i < argc; i++) {
        int n = snprintf(out + used, cap - used, "%s%s", i > 1 ? " " : "", argv[i]);
        if (n < 0 || (size_t)n >= cap - used) {
            return; // truncated: snprintf already ended `out` with '\0'
        }
        used += (size_t)n;
    }
}

int main(int argc, char **argv) {
    // Run this from your IDE with some "Program arguments" to see them here.
    printf("This program got %d argument(s):\n", argc - 1);
    for (int i = 1; i < argc; i++) {
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }

    // Don't change the tests!
    char *fake[] = {"prog", "hello", "world", NULL};
    char buf[64];
    CHECK_EQ_INT(count_args(3, fake), 2);
    join_args(3, fake, buf, sizeof buf);
    CHECK_EQ_STR(buf, "hello world");
    return clings_summary();
}
