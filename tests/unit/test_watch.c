#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "clings.h"
#include "buf.h"
#include "info.h"
#include "watch.h"

static void write_text(const char *path, const char *text) {
    if (!file_write(path, text, strlen(text))) die("cannot write %s", path);
}

/* Set path's mtime to one second after its current one. */
static void bump_mtime(const char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0) die("cannot stat %s", path);
    struct timespec ts[2];
    ts[0] = sb.st_atim;
    ts[1] = sb.st_mtim;
    ts[1].tv_sec += 1;
    if (utimensat(AT_FDCWD, path, ts, 0) != 0) die("cannot touch %s", path);
}

int main(void) {
    /* key mapping */
    CHECK(watch_key_action('n') == WATCH_NEXT);
    CHECK(watch_key_action('h') == WATCH_HINT);
    CHECK(watch_key_action('l') == WATCH_LIST);
    CHECK(watch_key_action('x') == WATCH_RESET);
    CHECK(watch_key_action('r') == WATCH_RERUN);
    CHECK(watch_key_action('q') == WATCH_QUIT);
    CHECK(watch_key_action(0x04) == WATCH_QUIT); /* Ctrl-D */
    CHECK(watch_key_action('z') == WATCH_NONE);
    CHECK(watch_key_action(0) == WATCH_NONE);

    /* snapshots, in a temp project root */
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof cwd)) return 1;
    char dir[] = "/tmp/clings_watch_XXXXXX";
    if (!mkdtemp(dir) || chdir(dir) != 0) return 1;
    CHECK(mkdir_p("exercises/t"));
    write_text("exercises/t/w.c", "int main(void) { return 0; }\n");
    write_text("exercises/t/helper.c", "int helper(void) { return 1; }\n");

    char *srcs[] = {"helper.c"};
    struct exercise_info ex;
    memset(&ex, 0, sizeof ex);
    ex.name = "w";
    ex.dir = "t";
    ex.path = "exercises/t/w.c";
    ex.sources = srcs;
    ex.n_sources = 1;

    struct watch_snapshot snap;
    watch_snapshot_take(&ex, &snap);
    CHECK_EQ_INT(snap.n, 2);
    CHECK(!watch_snapshot_changed(&ex, &snap));

    /* helper rewritten */
    write_text("exercises/t/helper.c", "int helper(void) { return 2; }\n");
    bump_mtime("exercises/t/helper.c");
    CHECK(watch_snapshot_changed(&ex, &snap));
    watch_snapshot_free(&snap);

    watch_snapshot_take(&ex, &snap);
    CHECK(!watch_snapshot_changed(&ex, &snap));

    /* exercise file deleted */
    unlink("exercises/t/w.c");
    CHECK(watch_snapshot_changed(&ex, &snap));
    watch_snapshot_free(&snap);
    CHECK(snap.mtimes == NULL && snap.n == 0);

    unlink("exercises/t/helper.c");
    rmdir("exercises/t");
    rmdir("exercises");
    if (chdir(cwd) != 0) return 1;
    rmdir(dir);
    return clings_summary();
}
