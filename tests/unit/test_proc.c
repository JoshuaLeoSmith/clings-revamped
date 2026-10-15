#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "clings.h"
#include "buf.h"
#include "proc.h"

static double now_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main(void) {
    struct proc_result r;

    /* normal exit status */
    {
        const char *const argv[] = {"sh", "-c", "exit 3", NULL};
        CHECK(proc_run(argv, NULL, NULL, 0, 4096, &r));
        CHECK(r.exited);
        CHECK_EQ_INT(r.status, 3);
        CHECK(!r.timed_out && !r.truncated);
        proc_result_free(&r);
        CHECK(r.output.data == NULL && r.output.len == 0 && !r.exited);
    }

    /* killed by a signal */
    {
        const char *const argv[] = {"sh", "-c", "kill -SEGV $$", NULL};
        CHECK(proc_run(argv, NULL, NULL, 0, 4096, &r));
        CHECK(!r.exited);
        CHECK_EQ_INT(r.signal, SIGSEGV);
        proc_result_free(&r);
    }

    /* timeout */
    {
        const char *const argv[] = {"sh", "-c", "sleep 5", NULL};
        double t0 = now_s();
        CHECK(proc_run(argv, NULL, NULL, 200, 4096, &r));
        double elapsed = now_s() - t0;
        CHECK(r.timed_out);
        CHECK(!r.exited);
        CHECK(elapsed < 1.0);
        proc_result_free(&r);
    }

    /* stdin text */
    {
        const char *const argv[] = {"cat", NULL};
        CHECK(proc_run(argv, NULL, "abc", 2000, 4096, &r));
        CHECK(r.exited);
        CHECK_EQ_INT(r.status, 0);
        CHECK_EQ_STR(r.output.data, "abc");
        proc_result_free(&r);
    }

    /* null stdin gives EOF */
    {
        const char *const argv[] = {"cat", NULL};
        CHECK(proc_run(argv, NULL, NULL, 2000, 4096, &r));
        CHECK(!r.timed_out);
        CHECK(r.exited);
        CHECK_EQ_INT(r.status, 0);
        CHECK_EQ_INT(r.output.len, 0);
        proc_result_free(&r);
    }

    /* stdout and stderr merged */
    {
        const char *const argv[] = {"sh", "-c", "echo out; echo err 1>&2", NULL};
        CHECK(proc_run(argv, NULL, NULL, 2000, 4096, &r));
        CHECK(r.exited && r.status == 0);
        CHECK(r.output.data && strstr(r.output.data, "out\n"));
        CHECK(r.output.data && strstr(r.output.data, "err\n"));
        proc_result_free(&r);
    }

    /* working directory */
    {
        const char *const argv[] = {"pwd", NULL};
        CHECK(proc_run(argv, "/tmp", NULL, 2000, 4096, &r));
        CHECK(r.exited && r.status == 0);
        CHECK_EQ_STR(r.output.data, "/tmp\n");
        proc_result_free(&r);
    }

    /* output cap */
    {
        const char *const argv[] = {"sh", "-c", "yes", NULL};
        CHECK(proc_run(argv, NULL, NULL, 5000, 1000, &r));
        CHECK(r.truncated);
        CHECK(!r.timed_out);
        CHECK(r.output.len <= 1000);
        CHECK(r.output.len > 0);
        proc_result_free(&r);
    }

    /* exec failure */
    {
        const char *const argv[] = {"/nonexistent/x", NULL};
        CHECK(proc_run(argv, NULL, NULL, 2000, 4096, &r));
        CHECK(r.exited);
        CHECK_EQ_INT(r.status, 127);
        CHECK(r.output.data && strstr(r.output.data, "cannot run"));
        proc_result_free(&r);
    }

    /* detached spawn */
    {
        char dir[] = "/tmp/clings_proc_XXXXXX";
        if (!mkdtemp(dir)) return 1;
        char file[512], cmd[600];
        snprintf(file, sizeof file, "%s/touched", dir);
        snprintf(cmd, sizeof cmd, "touch '%s'", file);
        const char *const argv[] = {"sh", "-c", cmd, NULL};
        CHECK(proc_spawn_detached(argv));
        bool seen = false;
        for (int i = 0; i < 100 && !seen; i++) {
            if (access(file, F_OK) == 0) {
                seen = true;
                break;
            }
            struct timespec ts = {0, 10 * 1000 * 1000};
            nanosleep(&ts, NULL);
        }
        CHECK(seen);
        unlink(file);
        rmdir(dir);
    }

    return clings_summary();
}
