#include "exercise.h"

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "proc.h"
#include "term.h"

#define BUILD_DIR ".clings-build"
#define MAX_OUTPUT 65536
#define DEFAULT_TIMEOUT_MS 5000

static const char *compiler(void) {
    const char *cc = getenv("CC");
    return cc && *cc ? cc : "cc";
}

void exercise_dir(const struct exercise_info *ex, struct buf *out) {
    buf_clear(out);
    buf_printf(out, "exercises/%s", ex->dir);
}

void exercise_solution_path(const struct exercise_info *ex, struct buf *out) {
    buf_clear(out);
    buf_printf(out, "solutions/%s/%s.c", ex->dir, ex->name);
}

void exercise_binary(const struct exercise_info *ex, bool solution, struct buf *out) {
    buf_clear(out);
    buf_printf(out, BUILD_DIR "/%s%s", ex->name, solution ? "_sol" : "");
}

bool exercise_compiler_available(void) {
    const char *const argv[] = {compiler(), "--version", NULL};
    struct proc_result r;
    if (!proc_run(argv, NULL, NULL, DEFAULT_TIMEOUT_MS, MAX_OUTPUT, &r)) return false;
    bool ok = r.exited && r.status == 0;
    proc_result_free(&r);
    return ok;
}

static const char *signal_name(int sig) {
    switch (sig) {
    case SIGSEGV: return "SIGSEGV";
    case SIGABRT: return "SIGABRT";
    case SIGFPE: return "SIGFPE";
    case SIGILL: return "SIGILL";
    case SIGBUS: return "SIGBUS";
    case SIGKILL: return "SIGKILL";
    case SIGTERM: return "SIGTERM";
    case SIGINT: return "SIGINT";
    case SIGPIPE: return "SIGPIPE";
    case SIGTRAP: return "SIGTRAP";
    default: return NULL;
    }
}

static void append_failure(struct buf *output, const struct proc_result *r, int timeout_ms) {
    if (output->len > 0 && output->data[output->len - 1] != '\n') buf_appends(output, "\n");
    term_color(output, TERM_RED);
    if (r->timed_out) {
        if (timeout_ms < 1000)
            buf_printf(output, "killed after %d ms, is there an infinite loop?", timeout_ms);
        else
            buf_printf(output, "killed after %d seconds, is there an infinite loop?",
                       timeout_ms / 1000);
    } else if (r->exited) {
        buf_printf(output, "exited with status %d", r->status);
    } else {
        const char *name = signal_name(r->signal);
        if (name)
            buf_printf(output, "crashed with %s", name);
        else
            buf_printf(output, "crashed with signal %d (%s)", r->signal, strsignal(r->signal));
    }
    if (r->truncated) buf_appends(output, " (output truncated at 64 KiB)");
    term_color(output, TERM_RESET);
    buf_appends(output, "\n");
}

bool exercise_check(const struct exercise_info *ex, const struct check_opts *opts,
                    struct buf *output) {
    static const struct check_opts defaults = {false, false, 0};
    if (!opts) opts = &defaults;
    int timeout_ms = opts->timeout_ms > 0 ? opts->timeout_ms : DEFAULT_TIMEOUT_MS;

    buf_clear(output);
    term_color(output, TERM_BOLD);
    term_color(output, TERM_UNDERLINE);
    buf_appends(output, "Output");
    term_color(output, TERM_RESET);
    buf_appends(output, "\n");

    struct buf dir, src, bin;
    buf_init(&dir);
    buf_init(&src);
    buf_init(&bin);
    exercise_dir(ex, &dir);
    if (opts->solution)
        exercise_solution_path(ex, &src);
    else
        buf_appends(&src, ex->path);
    exercise_binary(ex, opts->solution, &bin);

    /* Helper source paths: exercises/DIR/SOURCE */
    char **helpers = xmalloc((ex->n_sources ? ex->n_sources : 1) * sizeof *helpers);
    for (size_t i = 0; i < ex->n_sources; i++) {
        struct buf p;
        buf_init(&p);
        buf_printf(&p, "%s/%s", dir.data, ex->sources[i]);
        helpers[i] = p.data; /* ownership moves to helpers */
    }

    const char *no_sanitize = getenv("CLINGS_NO_SANITIZE");
    bool sanitize = !(no_sanitize && *no_sanitize);
    bool werror = ex->strict || opts->solution || opts->werror;

    size_t cap = 20 + ex->n_sources;
    const char **argv = xmalloc(cap * sizeof *argv);
    size_t n = 0;
    argv[n++] = compiler();
    argv[n++] = "-std=c11";
    argv[n++] = "-Wall";
    argv[n++] = "-Wextra";
    argv[n++] = "-Wshadow";
    argv[n++] = "-g";
    if (sanitize) {
        argv[n++] = "-fsanitize=address,undefined";
        argv[n++] = "-fno-sanitize-recover=all";
    }
    if (werror) argv[n++] = "-Werror";
    argv[n++] = "-I";
    argv[n++] = "include";
    argv[n++] = "-I";
    argv[n++] = dir.data;
    argv[n++] = "-o";
    argv[n++] = bin.data;
    argv[n++] = src.data;
    for (size_t i = 0; i < ex->n_sources; i++) argv[n++] = helpers[i];
    argv[n++] = "-lm";
    argv[n] = NULL;

    bool passed = false;
    struct proc_result cr;
    mkdir_p(BUILD_DIR);
    if (!proc_run(argv, NULL, NULL, 0, MAX_OUTPUT, &cr)) {
        buf_appends(output, "clings: cannot run the compiler\n");
    } else {
        buf_append(output, cr.output.data, cr.output.len);
        bool compiled = cr.exited && cr.status == 0;
        proc_result_free(&cr);
        if (compiled) {
            /* The binary runs with cwd exercises/DIR, so go up two levels. */
            struct buf exe;
            buf_init(&exe);
            buf_printf(&exe, "../../%s", bin.data);
            const char *const run_argv[] = {exe.data, NULL};
            struct proc_result rr;
            if (!proc_run(run_argv, dir.data, ex->stdin_text, timeout_ms, MAX_OUTPUT, &rr)) {
                buf_appends(output, "clings: cannot run the exercise\n");
            } else {
                buf_append(output, rr.output.data, rr.output.len);
                passed = rr.exited && rr.status == 0 && !rr.timed_out && !rr.truncated;
                if (!passed) append_failure(output, &rr, timeout_ms);
                proc_result_free(&rr);
            }
            buf_free(&exe);
        }
    }

    free(argv);
    for (size_t i = 0; i < ex->n_sources; i++) free(helpers[i]);
    free(helpers);
    buf_free(&dir);
    buf_free(&src);
    buf_free(&bin);
    return passed;
}
