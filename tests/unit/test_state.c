#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "clings.h"
#include "buf.h"
#include "info.h"
#include "state.h"

#define TOML "format_version = 1\n"                                       \
    "[[exercises]]\nname = \"a\"\ndir = \"d\"\nhint = \"h\"\n"            \
    "[[exercises]]\nname = \"b\"\ndir = \"d\"\nhint = \"h\"\n"            \
    "[[exercises]]\nname = \"c\"\ndir = \"d\"\nhint = \"h\"\n"            \
    "[[exercises]]\nname = \"d\"\ndir = \"d\"\nhint = \"h\"\n"

static char path[512];

static void put(const char *s) { CHECK(file_write(path, s, strlen(s))); }

static void check_file(const char *want) {
    struct buf b;
    buf_init(&b);
    CHECK(file_read(path, &b));
    CHECK(b.len == strlen(want) && memcmp(b.data, want, b.len) == 0);
    buf_free(&b);
}

int main(void) {
    char dir[] = "/tmp/clings_state_XXXXXX";
    if (!mkdtemp(dir)) return 1;
    snprintf(path, sizeof path, "%s/state.txt", dir);

    struct info_file info = {0};
    char err[256] = "";
    CHECK(info_parse(TOML, &info, err, sizeof err));

    struct app_state st;
    bool fresh;

    /* missing */
    state_load(&st, &info, path, &fresh);
    CHECK(fresh);
    CHECK_EQ_INT(st.current, 0);
    CHECK_EQ_INT(st.n_done, 0);
    state_free(&st);

    /* normal */
    put("DON'T EDIT THIS FILE!\n\nc\n\na\nd\n");
    state_load(&st, &info, path, &fresh);
    CHECK(!fresh);
    CHECK_EQ_INT(st.current, 2);
    CHECK(st.done[0] && st.done[3] && !st.done[1] && !st.done[2]);
    CHECK_EQ_INT(st.n_done, 2);

    /* wrap-around */
    size_t idx = 99;
    CHECK(state_next_pending(&st, &idx));
    CHECK_EQ_INT(idx, 1);
    /* save is byte-exact, in exercise order */
    CHECK(state_save(&st));
    check_file("DON'T EDIT THIS FILE!\n\nc\n\na\nd\n");
    state_free(&st);

    /* unknown done names ignored; order normalised on save */
    put("DON'T EDIT THIS FILE!\n\nb\n\nd\nzzz\na\n");
    state_load(&st, &info, path, &fresh);
    CHECK(!fresh);
    CHECK_EQ_INT(st.current, 1);
    CHECK_EQ_INT(st.n_done, 2);
    CHECK(state_save(&st));
    check_file("DON'T EDIT THIS FILE!\n\nb\n\na\nd\n");

    /* mutators save and round-trip */
    state_mark_done(&st, 1);
    state_mark_done(&st, 1);
    CHECK_EQ_INT(st.n_done, 3);
    state_mark_pending(&st, 0);
    state_mark_pending(&st, 0);
    CHECK_EQ_INT(st.n_done, 2);
    CHECK(state_set_current(&st, 2));
    CHECK(!state_set_current(&st, 99));
    CHECK_EQ_INT(st.current, 2);
    check_file("DON'T EDIT THIS FILE!\n\nc\n\nb\nd\n");
    state_free(&st);
    state_load(&st, &info, path, &fresh);
    CHECK(!fresh);
    CHECK_EQ_INT(st.current, 2);
    CHECK(st.done[1] && st.done[3] && !st.done[0]);
    CHECK_EQ_INT(st.n_done, 2);

    /* next_pending from 0 with only a done */
    state_mark_pending(&st, 1);
    state_mark_pending(&st, 3);
    state_mark_done(&st, 0);
    CHECK(state_set_current(&st, 0));
    CHECK(state_next_pending(&st, &idx));
    CHECK_EQ_INT(idx, 1);

    /* all done */
    for (size_t i = 0; i < 4; i++) state_mark_done(&st, i);
    CHECK_EQ_INT(st.n_done, 4);
    CHECK(!state_next_pending(&st, &idx));
    state_free(&st);

    /* empty and short files are fresh */
    put("");
    state_load(&st, &info, path, &fresh);
    CHECK(fresh);
    CHECK_EQ_INT(st.current, 0);
    CHECK_EQ_INT(st.n_done, 0);
    state_free(&st);
    put("DON'T EDIT THIS FILE!\n\n");
    state_load(&st, &info, path, &fresh);
    CHECK(fresh);
    state_free(&st);

    /* unknown current is fresh */
    put("DON'T EDIT THIS FILE!\n\nnope\n\na\n");
    state_load(&st, &info, path, &fresh);
    CHECK(fresh);
    CHECK_EQ_INT(st.n_done, 0);
    state_free(&st);

    /* unwritable path */
    state_load(&st, &info, "/nonexistent_dir_xyz/state.txt", &fresh);
    CHECK(!state_save(&st));
    state_free(&st);

    info_free(&info);
    unlink(path);
    rmdir(dir);
    return clings_summary();
}
