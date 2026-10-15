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
    // TODO: how many arguments are there, not counting argv[0]?
    (void)argc;
    (void)argv;
    return 0;
}

void join_args(int argc, char **argv, char *out, size_t cap) {
    // TODO: join argv[1..argc-1] into `out` with single spaces between them.
    (void)argc;
    (void)argv;
    if (cap > 0) {
        out[0] = '\0';
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
