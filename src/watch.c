#include "watch.h"

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "buf.h"
#include "commands.h"
#include "exercise.h"
#include "list.h"
#include "proc.h"
#include "term.h"

#define POLL_MS 250

enum watch_action watch_key_action(int key) {
    switch (key) {
    case 'n': return WATCH_NEXT;
    case 'h': return WATCH_HINT;
    case 'l': return WATCH_LIST;
    case 'x': return WATCH_RESET;
    case 'r': return WATCH_RERUN;
    case 'q': return WATCH_QUIT;
    case 0x04: return WATCH_QUIT; /* Ctrl-D: raw mode delivers VEOF as a byte */
    default: return WATCH_NONE;
    }
}

/* Path of file i of ex: 0 is the exercise, then exercises/DIR/SOURCE. */
static void snapshot_path(const struct exercise_info *ex, size_t i, struct buf *out) {
    if (i == 0) {
        buf_clear(out);
        buf_appends(out, ex->path);
        return;
    }
    exercise_dir(ex, out);
    buf_printf(out, "/%s", ex->sources[i - 1]);
}

/* mtime of path, or {0, -1} when it cannot be stat'ed. */
static struct timespec mtime_of(const char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0) return (struct timespec){0, -1};
    return sb.st_mtim;
}

void watch_snapshot_take(const struct exercise_info *ex, struct watch_snapshot *s) {
    s->n = 1 + ex->n_sources;
    s->mtimes = xmalloc(s->n * sizeof *s->mtimes);
    struct buf p;
    buf_init(&p);
    for (size_t i = 0; i < s->n; i++) {
        snapshot_path(ex, i, &p);
        s->mtimes[i] = mtime_of(p.data);
    }
    buf_free(&p);
}

bool watch_snapshot_changed(const struct exercise_info *ex, const struct watch_snapshot *s) {
    if (s->n != 1 + ex->n_sources) return true;
    struct buf p;
    buf_init(&p);
    bool changed = false;
    for (size_t i = 0; i < s->n && !changed; i++) {
        snapshot_path(ex, i, &p);
        struct timespec t = mtime_of(p.data);
        /* A file missing now differs from the recorded mtime unless it was
         * already missing then (avoids a rerun every poll while it stays gone). */
        changed = t.tv_sec != s->mtimes[i].tv_sec || t.tv_nsec != s->mtimes[i].tv_nsec;
    }
    buf_free(&p);
    return changed;
}

void watch_snapshot_free(struct watch_snapshot *s) {
    free(s->mtimes);
    s->mtimes = NULL;
    s->n = 0;
}

/* Watch-mode session state. */
struct watch {
    struct app_state *st;
    const struct cli *cli;
    struct watch_snapshot snap;
    struct buf output; /* last Output block */
    bool done;         /* last check passed */
    bool show_hint;
};

static const struct exercise_info *cur(const struct watch *w) {
    return &w->st->info->exercises[w->st->current];
}

/* Open the current exercise with --edit-cmd, split on spaces. */
static void open_editor(const struct watch *w) {
    if (!w->cli->edit_cmd) return;
    char *cmd = xstrdup(w->cli->edit_cmd);
    size_t cap = 2;
    for (const char *c = cmd; *c; c++)
        if (*c == ' ') cap++;
    const char **argv = xmalloc((cap + 1) * sizeof *argv);
    size_t n = 0;
    for (char *tok = strtok(cmd, " "); tok; tok = strtok(NULL, " ")) argv[n++] = tok;
    if (n) {
        argv[n++] = cur(w)->path;
        argv[n] = NULL;
        proc_spawn_detached(argv);
    }
    free(argv);
    free(cmd);
}

/* Set when drain_stdin swallowed Ctrl-D or hit EOF: the input is gone, so
 * watch mode must still end even though the key itself was discarded. */
static bool stdin_closed;

/* Discard keys typed while a check was running. */
static void drain_stdin(void) {
    struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
    char c;
    while (poll(&pfd, 1, 0) > 0 && (pfd.revents & (POLLIN | POLLHUP))) {
        if (read(STDIN_FILENO, &c, 1) != 1) {
            stdin_closed = true;
            break;
        }
        if (c == 0x04) stdin_closed = true;
    }
}

/* Bold key letter followed by ":label / " (or the final text). */
static void prompt_key(struct buf *out, const char *key, const char *rest) {
    term_color(out, TERM_BOLD);
    buf_appends(out, key);
    term_color(out, TERM_RESET);
    buf_appends(out, rest);
}

static void render(const struct watch *w) {
    const struct app_state *st = w->st;
    const struct exercise_info *ex = cur(w);
    struct buf out;
    buf_init(&out);
    term_clear(&out);
    if (w->output.len) {
        buf_append(&out, w->output.data, w->output.len);
        if (w->output.data[w->output.len - 1] != '\n') buf_appends(&out, "\n");
        buf_appends(&out, "\n");
    }
    if (w->show_hint) {
        term_color(&out, TERM_CYAN);
        term_color(&out, TERM_BOLD);
        term_color(&out, TERM_UNDERLINE);
        buf_appends(&out, "Hint");
        term_color(&out, TERM_RESET);
        buf_appends(&out, "\n");
        append_trimmed(&out, ex->hint);
        buf_appends(&out, "\n\n");
    }
    if (w->done) {
        term_color(&out, TERM_GREEN);
        term_color(&out, TERM_BOLD);
        buf_appends(&out, "Exercise done ✓");
        term_color(&out, TERM_RESET);
        buf_appends(&out, "\n");
        solution_line(ex, &out);
        buf_appends(&out, "When done experimenting, enter n to move on to the next exercise\n\n");
    }
    term_progress_bar(&out, st->n_done, st->info->n, term_width());
    buf_appends(&out, "\nCurrent exercise: ");
    term_color(&out, TERM_BLUE);
    term_color(&out, TERM_UNDERLINE);
    term_file_link(&out, ex->path);
    term_color(&out, TERM_RESET);
    buf_appends(&out, "\n\n");
    if (w->done) prompt_key(&out, "n", ":next / ");
    if (!w->show_hint) prompt_key(&out, "h", ":hint / ");
    prompt_key(&out, "l", ":list / ");
    prompt_key(&out, "x", ":reset / ");
    prompt_key(&out, "r", ":rerun / ");
    prompt_key(&out, "q", ":quit ? ");
    flush_out(&out);
    buf_free(&out);
}

/* Check the current exercise and render the result. */
static void run_current(struct watch *w) {
    const struct exercise_info *ex = cur(w);
    watch_snapshot_free(&w->snap);
    watch_snapshot_take(ex, &w->snap);
    printf("\nChecking the exercise %s. Please wait…\n", ex->name);
    fflush(stdout);
    w->done = exercise_check(ex, NULL, &w->output);
    if (!w->done) state_mark_pending(w->st, w->st->current);
    drain_stdin();
    render(w);
}

/* The current exercise changed: reset per-exercise view, open, run. */
static void switch_exercise(struct watch *w) {
    w->show_hint = false;
    open_editor(w);
    run_current(w);
}

static void quit_message(void) {
    fputs("\nWe hope you're enjoying learning C!\n"
          "If you want to continue working on the exercises at a later point, "
          "you can simply run clings again in this directory.\n",
          stdout);
    fflush(stdout);
}

/* Blocking read of one byte; -1 on EOF or error. */
static int read_key(void) {
    for (;;) {
        unsigned char c;
        ssize_t r = read(STDIN_FILENO, &c, 1);
        if (r == 1) return c;
        if (r < 0 && errno == EINTR) continue;
        return -1;
    }
}

/* Handle 'n'. Returns true when watch mode should end (all done). */
static bool next_exercise(struct watch *w) {
    struct app_state *st = w->st;
    state_mark_done(st, st->current);
    size_t next;
    if (state_next_pending(st, &next)) {
        state_set_current(st, next);
        switch_exercise(w);
        return false;
    }
    size_t before = st->current;
    fputs("\n", stdout);
    if (cmd_check_all(st) == 0) return true;
    drain_stdin();
    if (st->current != before)
        switch_exercise(w);
    else
        run_current(w);
    return false;
}

static void reset_exercise(struct watch *w) {
    const struct exercise_info *ex = cur(w);
    printf("\nResetting will undo all your changes to the file %s\nReset (y/n)? ", ex->path);
    fflush(stdout);
    int c = read_key();
    if (c == 'y' || c == 'Y') {
        fputs("\n", stdout);
        if (cmd_reset(w->st, ex->name) == 0) {
            run_current(w);
            return;
        }
        fputs("Press any key to continue", stdout);
        fflush(stdout);
        read_key();
    }
    render(w);
}

static void show_list(struct watch *w) {
    term_raw_off();
    struct buf out;
    buf_init(&out);
    term_clear(&out);
    list_render(w->st, &out);
    flush_out(&out);
    buf_free(&out);
    bool changed = list_prompt_jump(w->st);
    term_raw_on();
    if (changed)
        switch_exercise(w);
    else
        render(w);
}

int watch_run(struct app_state *st, const struct cli *cli) {
    struct watch w = {.st = st, .cli = cli};
    buf_init(&w.output);
    term_raw_on();
    switch_exercise(&w);

    int rc = 0;
    for (;;) {
        if (stdin_closed) { /* Ctrl-D or EOF arrived during a check */
            quit_message();
            break;
        }
        struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
        int r = poll(&pfd, 1, POLL_MS);
        if (r < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "clings: poll failed: %s\n", strerror(errno));
            rc = 2;
            break;
        }
        if (r == 0) {
            if (watch_snapshot_changed(cur(&w), &w.snap)) run_current(&w);
            continue;
        }
        int key = read_key();
        if (key < 0) { /* EOF on the terminal: leave as if q was pressed */
            quit_message();
            break;
        }
        enum watch_action act = watch_key_action(key);
        if (act == WATCH_NEXT && !w.done) act = WATCH_NONE;
        switch (act) {
        case WATCH_NONE: break;
        case WATCH_NEXT:
            if (next_exercise(&w)) goto out;
            break;
        case WATCH_HINT:
            w.show_hint = true;
            render(&w);
            break;
        case WATCH_LIST: show_list(&w); break;
        case WATCH_RESET: reset_exercise(&w); break;
        case WATCH_RERUN: run_current(&w); break;
        case WATCH_QUIT: quit_message(); goto out;
        }
    }
out:
    term_raw_off();
    watch_snapshot_free(&w.snap);
    buf_free(&w.output);
    return rc;
}
