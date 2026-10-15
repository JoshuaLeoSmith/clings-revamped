#ifndef CLINGS_PROC_H
#define CLINGS_PROC_H

#include <stdbool.h>
#include <stddef.h>
#include "buf.h"

struct proc_result {
    bool exited;      /* normal exit; status is valid */
    int status;       /* exit status when exited */
    int signal;       /* terminating signal when !exited */
    bool timed_out;   /* killed because timeout_ms elapsed */
    bool truncated;   /* killed because output reached max_output */
    struct buf output; /* merged stdout+stderr, at most max_output bytes */
};

/* Run argv[0] (PATH lookup) with argv, in cwd (NULL = inherit).
 * stdin_text NULL means stdin is /dev/null. timeout_ms <= 0 means no
 * timeout. Returns false only if pipe or fork fails; out is then zeroed. */
bool proc_run(const char *const argv[], const char *cwd,
              const char *stdin_text, int timeout_ms, size_t max_output,
              struct proc_result *out);

/* Free output and zero the struct. */
void proc_result_free(struct proc_result *r);

/* Start argv detached (double fork, stdio on /dev/null). Never waits for
 * it. Returns true if the process was forked. */
bool proc_spawn_detached(const char *const argv[]);

#endif
