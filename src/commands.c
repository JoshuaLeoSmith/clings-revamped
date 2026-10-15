#include "commands.h"

#include <stdio.h>
#include <string.h>
#include "buf.h"
#include "embedded.h"
#include "exercise.h"
#include "term.h"

static void flush_out(struct buf *out) {
    if (out->len) fwrite(out->data, 1, out->len, stdout);
    fflush(stdout);
    buf_clear(out);
}

/* Append s without leading and trailing whitespace. */
static void append_trimmed(struct buf *out, const char *s) {
    if (!s) return;
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    size_t n = strlen(s);
    while (n && (s[n - 1] == ' ' || s[n - 1] == '\t' || s[n - 1] == '\n' || s[n - 1] == '\r'))
        n--;
    buf_append(out, s, n);
}

/* Report an unknown exercise name with suggestions. */
static void report_unknown(const struct app_state *st, const char *name) {
    struct buf sugg;
    buf_init(&sugg);
    info_closest(st->info, name, &sugg);
    fprintf(stderr, "clings: no exercise named \"%s\"\n", name);
    if (sugg.len) fprintf(stderr, "Did you mean:\n%s", sugg.data);
    if (sugg.len && sugg.data[sugg.len - 1] != '\n') fputc('\n', stderr);
    buf_free(&sugg);
}

/* Select the exercise called name (if any). False after reporting an unknown name. */
static bool select_exercise(struct app_state *st, const char *name) {
    if (!name) return true;
    long idx = info_find(st->info, name);
    if (idx < 0) {
        report_unknown(st, name);
        return false;
    }
    state_set_current(st, (size_t)idx);
    return true;
}

static const struct exercise_info *current_ex(const struct app_state *st) {
    return &st->info->exercises[st->current];
}

/* "PREFIX<link>\n" */
static void file_link_line(struct buf *out, const char *prefix, const char *path) {
    buf_appends(out, prefix);
    term_file_link(out, path);
    buf_appends(out, "\n");
}

/* One check-all log line: green "   DONE name" or red "PENDING name". */
static void done_line(struct buf *out, const char *name, bool done) {
    term_color(out, done ? TERM_GREEN : TERM_RED);
    buf_appends(out, done ? "   DONE " : "PENDING ");
    buf_appends(out, name);
    term_color(out, TERM_RESET);
    buf_appends(out, "\n");
}

void render_final_message(const struct info_file *info, struct buf *out) {
    buf_appends(out,
                "+-----------------------------------------+\n"
                "|                                         |\n"
                "|     You made it to the finish line!     |\n"
                "|                                         |\n"
                "+-----------------------------------------+\n"
                "\n");
    append_trimmed(out, info->final_message);
    buf_appends(out, "\n");
}

int cmd_run(struct app_state *st, const char *name) {
    if (!select_exercise(st, name)) return 2;
    const struct exercise_info *ex = current_ex(st);

    struct buf out;
    buf_init(&out);
    bool ok = exercise_check(ex, NULL, &out);
    if (out.len && out.data[out.len - 1] != '\n') buf_appends(&out, "\n");

    if (!ok) {
        state_mark_pending(st, st->current);
        buf_appends(&out, "\nRan ");
        term_file_link(&out, ex->path);
        buf_appends(&out, " with errors\n");
        flush_out(&out);
        buf_free(&out);
        return 1;
    }

    buf_appends(&out, "\n");
    term_color(&out, TERM_GREEN);
    buf_printf(&out, "✓ Successfully ran %s", ex->path);
    term_color(&out, TERM_RESET);
    buf_appends(&out, "\n");

    struct buf sol;
    buf_init(&sol);
    exercise_solution_path(ex, &sol);
    if (file_exists(sol.data)) {
        buf_appends(&out, "\n");
        term_color(&out, TERM_BOLD);
        buf_appends(&out, "Solution");
        term_color(&out, TERM_RESET);
        buf_appends(&out, " for comparison: ");
        term_color(&out, TERM_CYAN);
        term_file_link(&out, sol.data);
        term_color(&out, TERM_RESET);
        buf_appends(&out, "\n");
    }
    buf_free(&sol);

    state_mark_done(st, st->current);
    size_t next;
    if (state_next_pending(st, &next)) {
        state_set_current(st, next);
        buf_appends(&out, "\n");
        file_link_line(&out, "Next exercise: ", st->info->exercises[next].path);
        flush_out(&out);
    } else {
        buf_appends(&out, "\n");
        flush_out(&out);
        cmd_check_all(st);
    }
    buf_free(&out);
    return 0;
}

int cmd_hint(struct app_state *st, const char *name) {
    if (!select_exercise(st, name)) return 2;
    const struct exercise_info *ex = current_ex(st);
    struct buf out;
    buf_init(&out);
    file_link_line(&out, "Current exercise: ", ex->path);
    buf_appends(&out, "\nHint:\n");
    append_trimmed(&out, ex->hint);
    buf_appends(&out, "\n");
    flush_out(&out);
    buf_free(&out);
    return 0;
}

int cmd_list(struct app_state *st) {
    struct buf out;
    buf_init(&out);
    for (size_t i = 0; i < st->info->n; i++) {
        buf_printf(&out, "%s%3zu  %s  %s\n", i == st->current ? "> " : "  ", i + 1,
                   st->done[i] ? "DONE   " : "PENDING", st->info->exercises[i].path);
    }
    flush_out(&out);
    buf_free(&out);
    return 0;
}

int cmd_check_all(struct app_state *st) {
    const struct info_file *info = st->info;
    struct buf out, discard;
    buf_init(&out);
    buf_init(&discard);

    buf_appends(&out, "Checking all exercises…\n");
    flush_out(&out);
    size_t n_pending = 0, first_pending = 0;
    for (size_t i = 0; i < info->n; i++) {
        bool ok = exercise_check(&info->exercises[i], NULL, &discard);
        if (ok) {
            state_mark_done(st, i);
        } else {
            state_mark_pending(st, i);
            if (!n_pending) first_pending = i;
            n_pending++;
        }
        done_line(&out, info->exercises[i].name, ok);
        buf_printf(&out, "Progress: %zu/%zu\n", i + 1, info->n);
        flush_out(&out);
    }
    buf_free(&discard);

    if (n_pending) {
        if (st->done[st->current]) state_set_current(st, first_pending);
        buf_appends(&out, "\n");
        const char *path = info->exercises[first_pending].path;
        if (n_pending == 1) {
            file_link_line(&out, "One exercise pending: ", path);
        } else {
            buf_printf(&out, "%zu/%zu exercises pending. The first: ", n_pending, info->n);
            term_file_link(&out, path);
            buf_appends(&out, "\n");
        }
        flush_out(&out);
        buf_free(&out);
        return 1;
    }

    if (term_is_tty())
        term_clear(&out);
    else
        buf_appends(&out, "\n");
    render_final_message(info, &out);
    flush_out(&out);
    buf_free(&out);
    return 0;
}

int cmd_reset(struct app_state *st, const char *name) {
    long idx = info_find(st->info, name);
    if (idx < 0) {
        report_unknown(st, name);
        return 2;
    }
    const struct exercise_info *ex = &st->info->exercises[idx];
    const char *text = embedded_lookup(ex->path);
    if (!text) {
        fprintf(stderr,
                "clings: no original is embedded for %s; run 'clings dev update' and rebuild\n",
                name);
        return 2;
    }
    if (!file_write(ex->path, text, strlen(text))) {
        fprintf(stderr, "clings: cannot write %s\n", ex->path);
        return 2;
    }
    struct buf out;
    buf_init(&out);
    buf_appends(&out, "The exercise ");
    term_file_link(&out, ex->path);
    buf_appends(&out, " has been reset\n");
    flush_out(&out);
    buf_free(&out);
    return 0;
}
