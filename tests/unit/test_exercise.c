#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "clings.h"
#include "buf.h"
#include "info.h"
#include "proc.h"
#include "term.h"
#include "exercise.h"

static void write_text(const char *path, const char *text) {
    if (!file_write(path, text, strlen(text))) die("cannot write %s", path);
}

/* Fills ex for exercises/t/<name>.c; strings point at static storage. */
static struct exercise_info make_ex(char *name, char *path) {
    struct exercise_info ex;
    memset(&ex, 0, sizeof ex);
    ex.name = name;
    ex.dir = "t";
    ex.path = path;
    return ex;
}

#define EX(var, nm) struct exercise_info var = make_ex(nm, "exercises/t/" nm ".c")

static bool contains(const struct buf *b, const char *s) {
    return b->data && strstr(b->data, s) != NULL;
}

/* Offset of s in b, or -1 when absent. */
static long find(const struct buf *b, const char *s) {
    const char *p = b->data ? strstr(b->data, s) : NULL;
    return p ? (long)(p - b->data) : -1;
}

int main(void) {
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof cwd)) return 1;

    struct buf hdr, out, path;
    buf_init(&hdr);
    buf_init(&out);
    buf_init(&path);
    buf_printf(&path, "%s/include/clings.h", cwd);
    if (!file_read(path.data, &hdr)) die("cannot read %s", path.data);

    char dir[] = "/tmp/clings_exercise_XXXXXX";
    if (!mkdtemp(dir)) return 1;
    if (chdir(dir) != 0) return 1;
    term_force_colors(false);

    CHECK(mkdir_p("include"));
    CHECK(mkdir_p("exercises/t"));
    CHECK(mkdir_p("solutions/t"));
    CHECK(file_write("include/clings.h", hdr.data, hdr.len));

    write_text("exercises/t/ok.c", "int main(void){return 0;}\n");
    write_text("exercises/t/bad.c", "int main(void){return 0}\n");
    write_text("exercises/t/exit1.c", "int main(void){return 1;}\n");
    write_text("exercises/t/warn.c", "int main(void){int unused; return 0;}\n");
    write_text("exercises/t/uses_helper.c",
               "int helper(void);\nint main(void){return helper();}\n");
    write_text("exercises/t/helper.c", "int helper(void){return 0;}\n");
    write_text("exercises/t/reads_stdin.c",
               "#include <stdio.h>\nint main(void){return getchar()=='x'?0:1;}\n");
    write_text("exercises/t/reads_file.c",
               "#include <stdio.h>\nint main(void){FILE *f = fopen(\"data.txt\", \"r\");"
               " if (!f) return 1; int c = fgetc(f); fclose(f); return c == 'd' ? 0 : 1;}\n");
    write_text("exercises/t/data.txt", "data\n");
    /* A literal null write is caught by UBSan ("runtime error: store to null
     * pointer") before ASan sees it; a near-null address segfaults instead. */
    write_text("exercises/t/segv.c",
               "int main(void){int *volatile p = (int *)16; *p = 1; return 0;}\n");
    write_text("exercises/t/flood.c",
               "#include <stdio.h>\nint main(void){for(;;) puts(\"flood\");}\n");
    write_text("exercises/t/loop.c", "int main(void){for(;;);}\n");
    write_text("exercises/t/noline.c",
               "#include <stdio.h>\nint main(void){printf(\"abc\"); return 1;}\n");
    write_text("exercises/t/sol.c", "int main(void){return 1;}\n");
    write_text("solutions/t/sol.c", "int main(void){return 0;}\n");
    write_text("exercises/t/uses_header.c",
               "#include \"clings.h\"\nint main(void){CHECK(1); return clings_summary();}\n");
    write_text("exercises/t/leak.c",
               "#include <stdlib.h>\n#include \"clings.h\"\n"
               "int main(void){printf(\"before leak\\n\"); CHECK_EQ_INT(1, 2);\n"
               "char *p = malloc(64); p[0] = 1; return clings_summary();}\n");

    /* path helpers */
    {
        EX(ex, "sol");
        buf_appends(&path, "junk");
        exercise_dir(&ex, &path);
        CHECK_EQ_STR(path.data, "exercises/t");
        exercise_solution_path(&ex, &path);
        CHECK_EQ_STR(path.data, "solutions/t/sol.c");
        exercise_binary(&ex, false, &path);
        CHECK_EQ_STR(path.data, ".clings-build/sol");
        exercise_binary(&ex, true, &path);
        CHECK_EQ_STR(path.data, ".clings-build/sol_sol");
    }

    CHECK(exercise_compiler_available());

    /* passes; output block starts with the heading */
    {
        EX(ex, "ok");
        buf_appends(&out, "stale");
        CHECK(exercise_check(&ex, NULL, &out));
        CHECK(strncmp(out.data, "Output\n", 7) == 0);
        CHECK(!contains(&out, "stale"));
        CHECK(file_exists(".clings-build/ok"));
    }

    /* compile error */
    {
        EX(ex, "bad");
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "error:"));
        CHECK(contains(&out, "exercises/t/bad.c:"));
        CHECK(!contains(&out, "exited with status"));
    }

    /* non-zero exit */
    {
        EX(ex, "exit1");
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "exited with status 1\n"));
    }

    /* warnings shown; strict turns them into errors */
    {
        EX(ex, "warn");
        CHECK(exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "warning:"));
        ex.strict = true;
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "error:"));
        ex.strict = false;
        struct check_opts o = {.werror = true};
        CHECK(!exercise_check(&ex, &o, &out));
    }

    /* helper sources */
    {
        EX(ex, "uses_helper");
        char *srcs[] = {"helper.c"};
        ex.sources = srcs;
        ex.n_sources = 1;
        CHECK(exercise_check(&ex, NULL, &out));
        ex.n_sources = 0;
        CHECK(!exercise_check(&ex, NULL, &out));
    }

    /* include path finds clings.h */
    {
        EX(ex, "uses_header");
        CHECK(exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "All 1 checks passed"));
    }

    /* stdin text, and /dev/null without it */
    {
        EX(ex, "reads_stdin");
        ex.stdin_text = "x";
        CHECK(exercise_check(&ex, NULL, &out));
        ex.stdin_text = NULL;
        CHECK(!exercise_check(&ex, NULL, &out));
    }

    /* cwd is the exercise directory */
    {
        EX(ex, "reads_file");
        CHECK(exercise_check(&ex, NULL, &out));
    }

    /* missing source file */
    {
        EX(ex, "missing");
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "No such file"));
    }

    /* sanitizer report */
    {
        EX(ex, "segv");
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "AddressSanitizer"));
    }

    /* output printed before a sanitizer exit is kept (stdout is a pipe) */
    {
        EX(ex, "leak");
        CHECK(!exercise_check(&ex, NULL, &out));
        long lsan = find(&out, "LeakSanitizer");
        long before = find(&out, "before leak");
        long fail = find(&out, "FAIL");
        CHECK(lsan >= 0);
        CHECK(before >= 0 && before < lsan);
        CHECK(fail >= 0 && fail < lsan);
    }

    /* without sanitizers the same program dies from a signal */
    {
        EX(ex, "segv");
        setenv("CLINGS_NO_SANITIZE", "1", 1);
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "crashed with SIGSEGV\n"));
        CHECK(!contains(&out, "AddressSanitizer"));
        unsetenv("CLINGS_NO_SANITIZE");
    }

    /* output cap */
    {
        EX(ex, "flood");
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "\nstopped after 64 KiB of output, is there an infinite loop?\n"));
        CHECK(!contains(&out, "SIGKILL"));
    }

    /* timeout */
    {
        EX(ex, "loop");
        struct check_opts o = {.timeout_ms = 500};
        CHECK(!exercise_check(&ex, &o, &out));
        CHECK(contains(&out, "killed after 500 ms"));
    }

    /* failure line starts on its own line */
    {
        EX(ex, "noline");
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "abc\nexited with status 1\n"));
    }

    /* solution build */
    {
        EX(ex, "sol");
        CHECK(!exercise_check(&ex, NULL, &out));
        struct check_opts o = {.solution = true};
        CHECK(exercise_check(&ex, &o, &out));
        CHECK(file_exists(".clings-build/sol_sol"));
    }

    /* compiler that cannot be started */
    {
        EX(ex, "ok");
        setenv("CC", "clings-no-such-compiler", 1);
        CHECK(!exercise_compiler_available());
        CHECK(!exercise_check(&ex, NULL, &out));
        CHECK(contains(&out, "clings: cannot run"));
        unsetenv("CC");
    }

    if (chdir(cwd) != 0) return 1;
    {
        const char *const argv[] = {"rm", "-rf", dir, NULL};
        struct proc_result r;
        CHECK(proc_run(argv, NULL, NULL, 10000, 4096, &r));
        CHECK(r.exited && r.status == 0);
        proc_result_free(&r);
    }

    buf_free(&hdr);
    buf_free(&out);
    buf_free(&path);
    return clings_summary();
}
