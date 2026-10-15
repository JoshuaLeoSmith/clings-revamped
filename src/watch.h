#ifndef CLINGS_WATCH_H
#define CLINGS_WATCH_H

#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include "info.h"
#include "main.h"
#include "state.h"

enum watch_action { WATCH_NONE, WATCH_NEXT, WATCH_HINT, WATCH_LIST, WATCH_RESET, WATCH_RERUN, WATCH_QUIT };

/* Map a key press to its action; unknown keys give WATCH_NONE. */
enum watch_action watch_key_action(int key);

/* Modification times of an exercise file followed by its helper sources. */
struct watch_snapshot {
    struct timespec *mtimes;
    size_t n;
};

void watch_snapshot_take(const struct exercise_info *ex, struct watch_snapshot *s);
/* True if any file's mtime differs from s, or a file is missing now that
 * existed when s was taken. */
bool watch_snapshot_changed(const struct exercise_info *ex, const struct watch_snapshot *s);
/* Free and zero s. */
void watch_snapshot_free(struct watch_snapshot *s);

/* Interactive watch mode on the terminal; returns the exit code. */
int watch_run(struct app_state *st, const struct cli *cli);

#endif
