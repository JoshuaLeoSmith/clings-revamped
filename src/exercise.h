#ifndef CLINGS_EXERCISE_H
#define CLINGS_EXERCISE_H

#include <stdbool.h>
#include "buf.h"
#include "info.h"

struct check_opts {
    bool solution;  /* build solutions/DIR/NAME.c instead of the exercise */
    bool werror;    /* add -Werror even when the exercise is not strict */
    int timeout_ms; /* 0 (or less) means 5000 */
};

/* Compile and run ex from the project root. Clears output, then fills it
 * with the "Output" block. True iff it compiled and exited with status 0.
 * opts may be NULL for defaults. */
bool exercise_check(const struct exercise_info *ex, const struct check_opts *opts,
                    struct buf *output);

/* Each clears out first. */
void exercise_solution_path(const struct exercise_info *ex, struct buf *out); /* solutions/DIR/NAME.c */
void exercise_dir(const struct exercise_info *ex, struct buf *out);           /* exercises/DIR */
void exercise_binary(const struct exercise_info *ex, bool solution,
                     struct buf *out); /* .clings-build/NAME[_sol] */

/* True iff `$CC --version` (default cc) exits with status 0. */
bool exercise_compiler_available(void);

#endif
